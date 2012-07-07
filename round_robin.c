#include <stdlib.h>

#include "main.h"

/**
 * insert_in_queue
 *
 * Implementación mínima de una cola en C
 * sobre un array dado el número total de elements
 * y el elemento a insertar
 */
void insert_in_queue(int* queue, int* tot_current, int element)
{
	queue[*tot_current] = element;

	// modificamos directamente tot_current
	*tot_current = *tot_current+1;
}

/**
 * get_first_queue
 *
 * Similar al pop de este tipo de estructuras,
 * obtenemos el primer elemento de la cola
 */
int get_first_queue(int* queue, int* tot_current)
{
	// guardamos el primer elemento
	int first_element = queue[0];
	int i;

	for(i=1; i<*tot_current; i++)
	{
		// movemos todos los elementos una
		// posición a la izquierda
		queue[i-1] = queue[i];
	}

	*tot_current = *tot_current-1;

	return first_element;
}

/**
 * round_robin_algorithm
 *
 * Dado un PLANNER_INPUT, devolvemos un
 * PLANNER_RESULT resultado de aplicar el algoritmo
 * round robin sobre él. Tiene 2 fases:
 *
 * 1f) Introducimos procesos a la cola hasta
 *	que hemos iterado una vez todos los procesos
 * 	y expropiamos debidamente
 *
 * 2f) Dejamos de introducir en la cola elementos
 *	y segumis expropiando
 *
 */
PLANNER_RESULT round_robin_algorithm(PLANNER_INPUT data)
{
	int i=0, j=0;
	PLANNER_RESULT rtemp;

	rtemp.num = data.num;

	float av_return = 0;
	float av_wait = 0;

	PLANNER_INPUT datacopy = data;

	// cuanto actual, obtenido del PLANNER_INPUT
	int current_quantum=data.quant;

	// proceso actual, -1 si no hemos empezamod a introducir
	// procesos en el algoritmo
	int current_process=-1;

	// variable que nos indica si estamos en la segunda
	// fase o no
	int second_phase=0;

	// suma de los servicios, iteraremos sobre este número
	int sum_servicio = sum_planner_data_stime(data);

	// creamos la cola con un número de elementos igual
	// al numero de procesos en total
	int queue[data.num];

	// hay 0 elementos en la cola
	int tot_current = 0;

	for(i; i<=sum_servicio; i++)
	{
		// el algoritmo empieza, planificamos el
		// primer proceso inmediatamente
		if(current_process == -1)
		{
			current_process = 0;
		}
		else
		{
			// en la segunda interación, podemos ya procesar
			data.times[current_process].t_servicio--;

			// restamos una unidad al cuanto
			current_quantum--;

			// si hemos acabado el proceso...
			if(data.times[current_process].t_servicio == 0)
			{
				rtemp.processes[current_process].TF = i+datacopy.idle;

				// ponemos el cuanto a 0 para comprobar a continuación que
				// proceso debemos planificar
				current_quantum = 0;
			}
		}

		// si no estamos en segunda fase, añadimos procesos
		// a la cola
		if(!second_phase)
		{
			if(current_process != i)
			{
				// add to the queue
				insert_in_queue(queue, &tot_current, i);
			}
		}

		// una vez los hemos añadido todos, entramos
		// en segunda fase
		if(i == datacopy.num-1)
		{
			second_phase = 1;
		}

		// expropiamos el proceso si ha terminado su cuanto
		// y si no es la última iteración
		if(current_quantum == 0 && i != sum_servicio)
		{
			// si hay 0 elementos en la cola, no hacemos nada
			if(tot_current != 0)
			{
				// guardamos el proceso actual
				int current_process_temp = current_process;

				// cogemos el primer elemento de la cola
				current_process = get_first_queue(queue, &tot_current);

				// añadimos el proceso a la cola, sólo si su tiempo de servicio
				// aún es mayor de 0
				if(data.times[current_process_temp].t_servicio  != 0)
					insert_in_queue(queue, &tot_current, current_process_temp);
			}

			// reseteamos el cuanto
			current_quantum = data.quant;
		}
	}

	// calculamos TR y TE
	for(i=0; i<data.num; i++)
	{
		rtemp.processes[i].TR = rtemp.processes[i].TF-datacopy.times[i].t_llegada;
		rtemp.processes[i].TE = rtemp.processes[i].TR-datacopy.times[i].t_servicio+datacopy.idle;

		av_return += rtemp.processes[i].TR;
		av_wait += rtemp.processes[i].TE;
	}

	// calculamos las medias
	rtemp.average_return = av_return / rtemp.num;
	rtemp.average_wait = av_wait / rtemp.num;

	return rtemp;
}
