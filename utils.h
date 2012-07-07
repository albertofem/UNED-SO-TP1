#include <stdio.h>

/**
 * Esta función se ha extraido de internet.
 * Elimina caracteres selectos de un C-string
 */
char* trim(char *s, const char *trimChars)
{
	char *start = s;

	while(*start && strpbrk((const char*)start, trimChars) == start)
		++start;

	char *i = start;
	char *end = start;

	while(*i)
	{
		if(strpbrk(i++, trimChars) != i-1)
		end = i;
	}

	*end = 0;

	return start;
}


/**
 * is_number
 *
 * Esta función comprueba si todos los
 * caracteres de un C-string son digitos
 */
int is_number(char string[])
{
	int i;

	for(i=0; i<strlen(string); i++)
	{
		if(!isdigit(string[i]))
		{
			// si este carácter no es un dígito
			// devolvemos false
			return 0;
		}
	}

	return 1;
}

/**
 * is_number_t
 *
 * Nos dice si un C-string es un número
 */
int is_number_t(char* s)
{
	if(is_number(trim(s, " \t\n\r")))
		return 1;

	return 0;
}


/**
 * bubble_sort
 *
 * Implementación del famoso bubble sort
 * para un PLANNER_INPUT
 *
 * http://en.wikipedia.org/wiki/Bubble_sort
 */
void bubble_sort(PLANNER_INPUT* input)
{
	int n = input->num;
	int i,j;

	for(i=0; i<n-1; i++)
	{
		for(j=0; j<(n-(i+1)); j++)
		{
			if(input->times[j].t_llegada > input->times[j+1].t_llegada)
			{
				// intercambiamos los datos
				int t_llegada = input->times[j].t_llegada;
				int t_servicio = input->times[j].t_servicio;

				input->times[j].t_llegada = input->times[j+1].t_llegada;
				input->times[j].t_servicio = input->times[j+1].t_servicio;

				input->times[j+1].t_llegada = t_llegada;
				input->times[j+1].t_servicio = t_servicio;
			}
		}
	}
}


/**
 * read_from_input
 *
 * Esta función se encarga de leer desde un buffer
 * de datos y almacenar en un C-string la línea que
 * acaba de leer. Se usa tanto para leer de 'stdin'
 * que no es más que un buffer de 1 línea, y para leer
 * desde ficheros de texto. En el caso de los ficheros
 * de texto, puesto que esta función opera sobre la
 * dirección de memoria del buffer directamente, al ser
 * llamada seguidamente, leerá una línea tras otra
 */
char* read_from_input(FILE* input)
{
	// reservamos en memoria una cantidad suficiente
	// para las operaciones de este programa
	char* line = malloc(256), *linep = line;
	size_t lenmax = 256, len = lenmax;
	int c;

	// buffer de datos terminado
	if(line == NULL)
		return NULL;

	// bucle ciego
	for(;;)
	{
		// obtener uns carácter
		c = fgetc(input);

		// si el carácter es EOF, hemos terminado
		// con este buffer
		if(c == EOF)
			return NULL;

		// si nos hemos pasado del buffer inicial
		// ampliamos la memoria disponible al doble
		// para poder seguir leyendo
		if(--len == 0)
		{
			// nuevo bloque de memoria
			char* linen = realloc(linep, lenmax *= 2);

			// actualizamos el máximo
			len = lenmax;

			// si no podemos reservar más memoria, liberamos
			// lo reservado e interrumpimos la lectura
			if(linen == NULL)
			{
				free(linep);
				return NULL;
			}

			// ahora la linea actual contiene
			// el nuevo bloque de memoria redimensionado
			line = linen + (line-linep);
			linep = linen;
		}

		// si la dirección de memoria siguiente
		// a este carácter es \n, terminamos de leer
		if((*line++ = c) == '\n')
			break;
	}

	// almacenamos EOL al final
	*line = '\0';

	// devolvemos la cadena sin retornos de carro, ni tabuladores, etc.
	return trim(linep, " \t\r\n");
}

/**
 * reset_planner_data
 *
 * Esta función resetea a 0 un PLANNER_RESULT
 * con un número específico de procesos
 */
PLANNER_RESULT reset_planner_data(PLANNER_RESULT data, int num)
{
	int i=0;

	for(i; i<num; i++)
	{
		data.processes[i].TE = 0;
		data.processes[i].TR = 0;
		data.processes[i].TF = 0;
	}

	return data;
}

/**
 * sum_planner_data_stime
 *
 * Esta función devuelve la suma de los servicios
 * de todos los procesos de un PLANNER_INPUT, puesto
 * que en los 3 algoritmos necesitamos saber a priori
 * cuánto va a tardar en procesarlos todos
 */
int sum_planner_data_stime(PLANNER_INPUT data)
{
	int i=0;
	int sum=0;

	for(i; i<data.num; i++)
	{
		sum += data.times[i].t_servicio;
	}

	return sum;
}

/**
 * print_planner_result
 *
 * Esta función imprime en un determinado buffer
 * la información de un PLANNER_RESULT
 */
void print_planner_result(PLANNER_RESULT result, FILE* target)
{
	int i=0;

	for(i; i<result.num; i++)
	{
		fprintf(target, PRINT_RESULT, i+1, result.processes[i].TF, result.processes[i].TR, result.processes[i].TE);
	}

	fprintf(target, PRINT_AVG, result.average_return, result.average_wait);
}

/**
 * procedimiento para hacer testeos
 */
void print_planner_input(PLANNER_INPUT input)
{
	int i=0;

	for(i; i<input.num; i++)
	{
		printf("Proceso %d, %d %d\n", i+1, input.times[i].t_llegada, input.times[i].t_servicio);
	}
}
