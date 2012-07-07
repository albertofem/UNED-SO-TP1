#include <stdlib.h>

#include "main.h"
#include "utils.h"

/**
 * srt_algorithm
 *
 * Obtiene un PLANNER_RESULT resultado de aplicar
 * el algoritmo SRT a un PLANNER_INPUT
 *
 * El algoritmo está dividido en 2 fases:
 *
 * 1f) iteramos sin tener en cuenta todos los procesos
 * 	que puede haber en la estructura, solamente aquellos
 * 	que ya han entrado en el algoritmo, pero están en cola
 *
 * 2f) iteramos sobre todos los procesos que quedan
 * 	seleccionando en cada iteración aquel con menos tiempo
 * 	de proceso
 */
PLANNER_RESULT srt_algorithm(PLANNER_INPUT data)
{
	int i=0, j=0;

	// proceso inicial
	int current_process=0;

	// nos indica si estamos en la primera fase
	int token_second=1;

	// nos indica en qué fase estamos
	int second_phase=0;

	// límite de iteraciones para procesos en cola
	int limit_iteration=0;

	// hacemos una copia del PLANNER_INPUT inicial
	// para operar sobre él en el algoritmo
	PLANNER_INPUT datacopy = data;

	// reseteamos la información del PLANNER_RESULT final
	PLANNER_RESULT rtemp = reset_planner_data(rtemp, data.num);
	rtemp.num = data.num;

	float av_return = 0;
	float av_wait = 0;

	// obtenemos la suma total de los servicios
	// puesto que iteraremos sobre este número
	int sum_servicio = sum_planner_data_stime(data);

	for(i; i<=sum_servicio; i++)
	{
		token_second = 1;

		// primero comprobamos si este proceso no ha terminado ya
		if(data.times[current_process].t_servicio != 0)
		{
			// ejecutamos una iteración de este proceso, y le restamos
			// tiempo de servicio
			data.times[current_process].t_servicio--;

			// si el proceso ha terminado, determinamos su tiempo de finalización
			if(data.times[current_process].t_servicio == 0)
			{
				rtemp.processes[current_process].TF = i+datacopy.idle+1;
			}
		}

		// buscamos el siguiente proceso con menos tiempo de servicio restante
		// si estamos en primera fase, sólo tenemos que buscar sobre aquellos
		// procesos que hayan entrado en el planificador
		//
		// en la segunda fase tenemos que buscar entre todos aquellos procesos
		// restantes, el de menos tiempo de servicio restante
		if(second_phase == 0)
			limit_iteration = i+2;
		else
			limit_iteration = datacopy.num;

		// iteramos el resto de procesos
		for(j=0; j<limit_iteration; j++)
		{
			if(second_phase == 1)
			{
				// segunda fase
				if(data.times[j].t_servicio > 0)
				{
					if(token_second)
					{
						current_process = j;
						token_second = 0;
					}

					// comprobamos las condiciones para la expropiación:
					// que el tiempo de servicio restante de este proceso sea menor
					// que el actual, y que el proceso que estamos comprobando no
					// sea el actual
					if(data.times[j].t_servicio < data.times[current_process].t_servicio
						&& data.times[j].t_servicio != data.times[current_process].t_servicio)
					{
						// expropiamos
						current_process = j;
					}
				}
			}
			else
			{
				// comprobamos las condiciones para la expropiación
				// el proceso tiene que tener tiempo de servicio > 0, si su tiempo
				// de servicio es diferente de 0, tiene que comprobar si el tiempo de
				// servicio restante del proceso a comprobar es menor, y además que
				// el proceso actual no sea el que estamos comprobando
				if(data.times[j].t_servicio > 0 &&
					(data.times[current_process].t_servicio != 0 ? data.times[j].t_servicio < data.times[current_process].t_servicio : 1) &&
					j != current_process)
				{
					// expropiamos
					current_process = j;
				}
			}
		}

		// cuando hemos alcanzado el número de total
		// de procesos, entramos en segunda fase
		if(i == datacopy.num-1)
		{
			second_phase = 1;
		}
	}


	// calculamos TR y TE, similar en todos los algoritmos
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

// unit test
void test_srt()
{
	PLANNER_INPUT test;

	test.times[0].t_llegada = 0;
	test.times[0].t_servicio = 5;

	test.times[1].t_llegada = 1;
	test.times[1].t_servicio = 3;

	test.times[2].t_llegada = 2;
	test.times[2].t_servicio = 4;

	test.times[3].t_llegada = 3;
	test.times[3].t_servicio = 2;

	test.num = 4;

	PLANNER_RESULT test2 = srt_algorithm(test);

	print_planner_result(test2, stdout);
}
