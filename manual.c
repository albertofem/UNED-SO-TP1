#include <stdio.h>
#include <ctype.h>

#include "main.h"

/**
 * manual_entry_read
 *
 * Obtenemos los datos desde el usuario
 * y lo almacenamos en un PLANNER_INPUT
 */
PLANNER_INPUT manual_entry_read()
{
	PLANNER_INPUT rtemp;
	int i=0, j;
	char* pro_r;
	int status=1;

	// iteramos hasta el límite de procesos
	while(i <= MAX_PROCESS-1)
	{
		// pedimos al usuario los 2 datos necesarios
		for(j=0; j<2; j++)
		{
			if(j==0)
				printf(MAN_IN, MAN_LL, i+1);
			else
				printf(MAN_IN, MAN_S, i+1);

			pro_r = read_from_input(stdin);

			// si no es un número, volvemos a pedir los
			// datos usando un break;
			if(!is_number_t(pro_r))
			{
				printf(MAN_ERROR_N);
				status = 0;

				break;
			}
			else
			{
				// datos correctos, los guardamos
				if(j == 0)
					rtemp.times[i].t_llegada = atoi(pro_r);
				else
					rtemp.times[i].t_servicio = atoi(pro_r);

				status = 1;
			}
		}

		// los datos no eran correctos, continue
		// y volvemos a pedirlos
		if(!status)
			continue;

		printf(MAN_MORE);

		pro_r = read_from_input(stdin);

		// comprobamos si el usuario quiere seguir
		// introduciendo datos, si no quiere...
		if(strcmp(pro_r, "y") != 0)
		{
			printf(MAN_RR);

			// pedimos el cuanto hasta que haya
			// una cantidad correcta
			while(1)
			{
				pro_r = read_from_input(stdin);

				// el cuanto no es un número, continue..
				if(!is_number_t(pro_r))
				{
					printf(MAN_ERROR_N);
					continue;
				}
				else
				{
					// cuanto correcto, lo almacenamos
					rtemp.quant = atoi(pro_r);
					break;
				}
			}

			// ya tenemos el cuanto almacenado, no hacemos
			// nada en este bucle, break...
			break;
		}

		i++;
	}

	// almacenamos el número de procesos
	rtemp.num = i+1;

	// y el tiempo de espera inicial
	rtemp.idle = rtemp.times[0].t_llegada;

	return rtemp;
}

/**
 * manual_entry
 *
 * Procedimiento para pedir al usuario
 * todos los datos, que posteriormente
 * procesaremos e imprimiremos, similar
 * al modo de lectura de un fichero
 */
void manual_entry()
{
	PLANNER_INPUT rtemp;

	rtemp = manual_entry_read();

	printf(INFO_OPTIONFILE);

	char* opt = read_from_input(stdin);

	if(strcmp(opt, "v") == 0 || strcmp(opt, "c") == 0)
	{
		// mostrar datos
		output_algorithm_result(&rtemp, stdout);
	}

	if(strcmp(opt, "w") == 0 || strcmp(opt, "c") == 0)
	{
		// guardar en fichero
		save_to_file(&rtemp);
	}

	return 0;
}

