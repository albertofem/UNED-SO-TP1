#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "main.h"

int main(int argc, char* argv[])
{
	// esto estaba para cargar la interfaz gráfica
	// desde línea de comandos, pero no me dio tiempo
	// a terminar esta implementación
	if(proccess_arguments(argc, argv))
	{
		// load_gui(argc, argv);
		printf(EN_FIN);
	}
	else
	{
		// procesamos las opciones hasta que haya una correcta
		int options = proccess_options(1);

		while(options == -1)
		{
			options = proccess_options(0);
		}

		// este switch controla la opción que lanzamos
		switch(options)
		{
		case 0:
			// leer desde un fichero
			readfile_input();
			break;

		case 1:
			// introducir los datos a mano
			manual_entry();
			break;

		case 2:
			// la opción de interfaz gráfica no está disponible
			printf(EN_FIN);
			break;
		}
	}

	return 0x0;
}
