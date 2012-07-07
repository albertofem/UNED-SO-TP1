#include <stdio.h>
#include <time.h>

#include "main.h"

/**
 * process_arguments
 *
 * Esta función procesa los argumentos por línea
 * de comandos y devuelve estados asociados a cada
 * opción, para ser gestionada externamente
 */
int proccess_arguments(int argc, char* argv[])
{
	if(argc > 1)
	{
		if(strcmp(argv[1], "-gui") == 0)
		{
			printf(EN_FIN);
			exit(0);
		}
		else if(strcmp(argv[1], "-h") == 0)
		{
			printf(INFO_HELP);
			return 0;
		}
		else
		{
			printf(ERROR_ARG, argv[1]);
			exit(0x255);
		}
	}

	return 0;
}

/**
 * process_options
 *
 * Esta función procesa las opciones introducidas
 * en 'stdin', y devuelve estados para ser gestionados
 * externamente
 */
int proccess_options(int show_info)
{
	if(show_info)
		printf(INFO_START);

	char* opt = read_from_input(stdin);

	if(opt != NULL)
	{
		// check options
		if(strcmp(opt, "a") == 0)
		{
			// leer desde fichero, estado 0
			return 0;
		}
		else if(strcmp(opt, "m") == 0)
		{
			// entrar datos a mano, estado 1
			return 1;
		}
		else if(strcmp(opt, "e") == 0)
		{
			// salir
			exit(0x0);
		}
		else if(strcmp(opt, "gui") == 0)
		{
			// lanzar interfaz gráfica, estado 2
			return 2;
		}
		else if(strcmp(opt, "easter egg") == 0)
		{
			printf(FUU);
		}
		else
		{
			// opción incorrecta, estado -1
			printf(ERROR_OPTION, opt);
			return -1;
		}
	}

	return -1;
}
