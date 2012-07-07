#include <stdio.h>
#include <ctype.h>

#include "main.h"

/**
 * Forward declaration
 */
int input_file(char* filename);
PLANNER_INPUT process_file(FILE* file);
PLANNER_RESULT srt_algorithm(PLANNER_INPUT data);
PLANNER_RESULT round_robin_algorithm(PLANNER_INPUT data);
void save_to_file(PLANNER_INPUT* input);

/**
 * readfile_input
 *
 * Esta función es el nexo de entrada para las
 * opciones a la hora de leer un fichero
 */
void readfile_input()
{
	int status = -1;

	while(status == -1)
	{
		printf(INFO_FILENAME);

		char* fname;
		fname = read_from_input(stdin);

		// comprobación de seguridad
		if(fname != NULL)
		{
			// para salir del bucle
			if(strcmp(fname, "e") == 0)
			{
				return;
			}

			// leemos el fichero, si el status devuelto
			// es != -1, el bucle terminará y el programa
			// acabará
			status = input_file(fname);
		}
		else
		{
			// fatal error
			exit(0x255);
		}
	}
}

/**
 * ouput_algorith_result
 *
 * Esta función llama a los 3 algoritmos sobre un
 * PLANNER_INPUT e imprime el resultado en un buffer
 * de datos, que bien puede ser 'stdout' o un fichero
 */
void output_algorithm_result(PLANNER_INPUT* data_input, FILE* target)
{
	PLANNER_RESULT rtemp;

	fprintf(target, INFO_RESULT);
	fprintf(target, INFO_RESULT_SEPARATOR);
		fprintf(target, INFO_RESULT_FCFS);
	fprintf(target, INFO_RESULT_SEPARATOR);

	// fcfs
	rtemp = fcfs_algorithm(*data_input);

	print_planner_result(rtemp, target);

	fprintf(target, INFO_RESULT_SEPARATOR);
		fprintf(target, INFO_RESULT_SRT);
	fprintf(target, INFO_RESULT_SEPARATOR);

	// srt
	rtemp = srt_algorithm(*data_input);

	print_planner_result(rtemp, target);

	fprintf(target, INFO_RESULT_SEPARATOR);
		fprintf(target, INFO_RESULT_RR, data_input->quant);
	fprintf(target, INFO_RESULT_SEPARATOR);

	// rr
	rtemp = round_robin_algorithm(*data_input);

	print_planner_result(rtemp, target);
}

/**
 * input_file
 *
 * Función principal para operar sobre el fichero de texto
 *
 * Primero intentamos leer el fichero, a continuación
 * lo procesamos y pedimos las opciones correspondientes
 * al usuario
 */
int input_file(char* filename)
{
	FILE *ifile;
	ifile = fopen(filename, "r");

	PLANNER_INPUT data_input;

	// fichero inaccesible
	if(ifile == NULL)
	{
		printf(ERROR_FILEOPEN, filename);
		return -1;
	}

	// fichero abierto, podemos proseguir
	data_input = process_file(ifile);

	printf(INFO_OPTIONFILE);

	char* opt = read_from_input(stdin);

	// tanto si es 'v' como 'c', imprimimos la información
	if(strcmp(opt, "v") == 0 || strcmp(opt, "c") == 0)
	{
		// imprimir en 'stdout'
		output_algorithm_result(&data_input, stdout);
	}

	// si es 'w' o 'c', escribimos la información en un fichero
	if(strcmp(opt, "w") == 0 || strcmp(opt, "c") == 0)
	{
		save_to_file(&data_input);
	}

	return 0;
}

/**
 * save_to_file
 *
 * Función para guardar en un fichero
 * el resultado de aplicar los algoritmos
 * a un PLANNER_INPUT
 */
void save_to_file(PLANNER_INPUT* input)
{
	printf(INFO_SAVEFILE);
	char* file_name = read_from_input(stdin);

	// tratamos de abrir el fichero
	FILE* target_file = fopen(file_name, "w");

	// mientras no podamos abrirlo vamos a seguir
	// solicitando el fichero
	while(target_file == NULL)
	{
		printf(ERROR_SAVEFILE);
		printf(INFO_SAVEFILE);

		file_name = read_from_input(stdin);
		target_file = fopen(file_name, "w");
	}

	// guardamos el resultado en el 'target_file'
	output_algorithm_result(input, target_file);
	fclose(target_file);

	printf(INFO_SUCCESS_WRITE, file_name);
}

/**
 * get_int_from_line
 *
 * Función para obtener un 'int' desde un C-string
 * especificando además el rango de posiciones
 */
int get_int_from_line(char* line, int s, int e)
{
	int length = e-s;
	char i_temp[length+1];
	int i=0, j=0;

	// si no vamos a empezar desde el principio
	// movemos el buffer 'line' 's' posiciones
	if(s != 0)
	{
		for(j; j<=s; j++)
			line++;
	}

	// hacemos una copia del carácter
	strncpy(i_temp, line, length);

	// añadimos EOL
	i_temp[length] = '\0';

	// si no es un número, esto no tiene sentido.
	// devolvemos el estado -1
	if(!is_number(trim(i_temp, " \t\n\r")))
		return -1;

	// puesto que es un número, lo convertimos
	// usando atoi()
	return atoi(i_temp);
}

/**
 * process_file
 *
 * Procesado del fichero una vez abierto, para obtener
 * línea por línea todos los datos correspondientes
 * a los procesos, con comprobación de errores, etc.
 */
PLANNER_INPUT process_file(FILE* file)
{
	char* line = read_from_input(file);
	int i=0, line_length, num_linea=1;
	char* line_temp;

	PLANNER_INPUT data_input;

	printf(INFO_PARSINGFILE);

	// bucle por líneas
	while(line != NULL)
	{
		// obtenemos la longitud de la línea
		line_length = strlen(line);

		printf(INFO_PARSINGTEXT, num_linea, line);

		// pasamos ahora a analizar la línea carácter por carácter
		for(i=0; i<line_length; i++)
		{
			if(line[i] == ' ')
			{
				// hemos encontrado un espacio en blanco,
				// tenemos que extraer el número a la izquierda
				// y el número a la derecha
				int uno = get_int_from_line(line, 0, i);
				int dos = get_int_from_line(line, i, line_length);

				// si alguno de estos no es un número, terminamos la
				// ejecución
				if(uno == -1)
				{
					printf(ERROR_LINEDIGIT, num_linea, INFO_FIRSTC);
					exit(0x255);
				}

				if(dos == -1)
				{
					printf(ERROR_LINEDIGIT, num_linea, INFO_SECONDC);
					exit(0x255);
				}

				// ambos son números, añadimos el proceso a la estructura
				data_input.times[num_linea-1].t_llegada = uno;
				data_input.times[num_linea-1].t_servicio = dos;
			}
		}

		// copiamos la linea anterior, que nos servirá para compromar
		// el cuanto al final del fichero
		line_temp = line;

		// leemos una nueva línea
		line = read_from_input(file);

		// la línea es NULL, por lo tanto estamos
		// al final del fichero
		if(line == NULL)
		{
			// vemos si el cuanto existe
			if(line_temp[0] == 'q')
			{
				if(line_temp[1] != '=')
				{
					// no está bien la cadena del cuanto, terminamos...
					printf(ERROR_QUANT);
					exit(0x255);
				}
				else
				{
					// la cadena parece correcta, intentamos
					// sacar el cuanto...
					int quantum = get_int_from_line(line_temp, 1, line_length);

					// vaya hombre... el cuanto no es un número, terminamos
					if(quantum == -1 || quantum == 0)
					{
						// bad quantum
						printf(ERROR_QUANT);
						exit(0x255);
					}
					else
					{
						// yes! el cuanto es un número, lo almacenamos
						data_input.quant = quantum;
					}
				}
			}
			else
			{
				printf(ERROR_QUANT_NE);
				exit(0);
			}
		}
		else
		{
			num_linea++;
		}
	}

	// almacenamos el número de procesos
	data_input.num = num_linea-1;

	// ordenamos los procesos según su tiempo de llegada
	bubble_sort(&data_input);

	// además, el tiempo de espera de la CPU
	// corresponde al tiempo de llegada del primer proceso
	data_input.idle = data_input.times[0].t_llegada;

	return data_input;
}
