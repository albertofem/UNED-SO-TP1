#include <stdlib.h>

#include "main.h"

/**
 * fcfs_algorithm
 *
 * Devuelve un PLANNER_RESULT resultado de
 * haber ejecutado el algoritmo FCFS sobre
 * un PLANNER_INPUT
 */
PLANNER_RESULT fcfs_algorithm(PLANNER_INPUT data)
{
	int i=0;
	PLANNER_RESULT rtemp;

	rtemp.num = data.num;

	float av_return = 0;
	float av_wait = 0;

	int t_fin=0;

	// hacemos tantas iteraciones como procesos haya
	for(i; i<data.num; i++)
	{
		// sumamos el tiempo final de cada proceso
		// que también nos sirve para la suma final
		t_fin += data.times[i].t_servicio;

		// el tiempo final corresponde a lo que llevamos
		// sumado en t_fiǹ + el tiempo de espera de la CPU
		rtemp.processes[i].TF = t_fin+data.idle;

		// obtenemos TR y TE
		rtemp.processes[i].TR = rtemp.processes[i].TF-data.times[i].t_llegada;
		rtemp.processes[i].TE = rtemp.processes[i].TR-data.times[i].t_servicio+data.idle;

		// sumamos el total de TR y TE
		av_return += rtemp.processes[i].TR;
		av_wait += rtemp.processes[i].TE;
	}

	// calculamos las medias
	rtemp.average_return = av_return / rtemp.num;
	rtemp.average_wait = av_wait / rtemp.num;

	return rtemp;
}

// unit testing
void test_fcfs()
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

	PLANNER_RESULT test2 = fcfs_algorithm(test);

	print_planner_result(test2);
}
