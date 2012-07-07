/**
 * Mensajes
 *
 * Aquí están contenidos todos los mensajes de
 * información, error, etc. del programa
 */

#define INFO_HELP "TP1-Procesos - 1.0 - Alberto Fernández Martínez\nAsignatura Sistemas Operativos (UNED) - Curso 2011-2012\n\nusage: TP1-Procesos [arguments]\n\nArguments:\n\t-gui\t\tLoad up graphical user interface\n\t-h\t\tShows this help\n"

#define INFO_START "******************************************************\n* Bienvenido al primer trabajo de Sistemas Operativos\n******************************************************\n* Introduce una de las siguientes opciones:\n*\t- Cargar datos desde un archivo de texto (a)\n*\t- Introducir datos manualmente (m)\n*\t- Salir del programa (e)\n******************************************************\n* Opción: "

#define INFO_PARSINGFILE "* Cargando datos desde el fichero de texto...\n*\n"

#define INFO_PARSINGTEXT "* Procesando línea %d: '%s'...\n"

#define EN_FIN "* Interfaz gráfica no implementada por falta de tiempo\n"

// Error messages
#define ERROR_ARG "TP1-Procesos: invalid option -- '%s'\nUsage: TP1-Procesos [-gui] [-h]\n"
#define ERROR_OPTION "* Opción inválida, vuelve a introducirla ('e' para salir): "
#define ERROR_FILEOPEN "* El archivo especificado '%s' no se pudo abrir.\n"

#define FUU "* Let's get back to work... (opción): "

#define ERROR_LINEDIGIT "*\n* Error en línea %d: formato incorrecto en la %s columna. \n* Corriga la entrada y vuelva a intentarlo\n*\n* Saliendo del programa...\n******************************************************\n"

#define INFO_FIRSTC "primera"
#define INFO_SECONDC "segunda"

#define INFO_FILENAME "* Introduzca el nombre del archivo: "
#define INFO_SAVEFILE "* Introduce el nombre del archivo a guardar: "
#define ERROR_SAVEFILE "* No se pudo acceder al fichero para su escritura, especifique otro\n"

#define INFO_OPTIONFILE "******************************************************\n* Gracias, sus datos han sido cargados correctamente:\n*\t- Ver por pantalla 'v'\n*\t- Escribir en fichero de texto 'w'\n*\t- Ver, escribir y salir 'c'\n******************************************************\n* Opción: "

#define INFO_RESULT "*******************************\n* RESULTADOS DE LA PLANIFICACIÓN"

#define INFO_RESULT_FCFS "* Planificación FCFS"
#define INFO_RESULT_SRT "* Planificación SRT"
#define INFO_RESULT_RR "* Planificación Round Robin con cuanto q=%d"

#define INFO_RESULT_SEPARATOR "\n*******************************\n"

#define INFO_SUCCESS_WRITE "******************************************************\n* Gracias, los datos se han guardado correctamente en el fichero '%s'\n******************************************************\n"

#define MAN_IN "* Introduzca el tiempo de %s del proceso %d: "
#define MAN_LL "llegada"
#define MAN_S "servicio"

#define MAN_ERROR_N "* ERROR! Debe especificar un número correcto\n"

#define MAN_MORE "* ¿Desea introducir más procesos? (y/n): "

#define MAN_RR "* Introduzca el valor del cuanto para el algoritmo Round Robin: "

#define ERROR_QUANT "* El cuanto especificado no es correcto\n"
#define ERROR_QUANT_NE "* El cuanto no existe en el fichero\n"

#define PRINT_RESULT "Proceso %d: Tf=%d Tr=%d Te=%d\n"
#define PRINT_AVG "\nTiempo promedio de retorno = %.2f\nTiempo promedio de espera = %.2f\n"

#define MAX_PROCESS 20

/**
 * Estructuras de datos
 *
 * Estas son las estructuras de datos que
 * usaremos a lo largo del programa
 */

/**
 * FILE_STATUS
 *
 * Esta estructura se usa para cuando queremos
 * abrir un archivo, podemos devolver el estado
 * y el nombre del fichero
 */
struct FILE_STATUS_s
{
	int status;
	char* filename;
};

typedef struct FILE_STATUS_s FILE_STATUS;

/**
 * PROCESS
 *
 * Contiene la información necesaria de cada proceso
 *
 * TF = Tiempo de finalizacion
 * TR = Tiempo de retorno
 * TE = Tiempo de espera
 */
struct PROCESS_s
{
	int TF;
	int TR;
	int TE;
};

/**
 * PLANNER_RESULT
 *
 * Contiene la información del resultado de
 * ejecutar un algoritmo de planificación sobre un
 * PLANNER_INPUT; contiene un array de 20 PROCESS
 * para los procesos, 2 float para las medias de
 * espera y retorno y un int para el número de procesos
 * en total
 */
struct PLANNER_RESULT_s
{
	struct PROCESS_s processes[MAX_PROCESS];
	float average_return;
	float average_wait;

	int num;
};

typedef struct PLANNER_RESULT_s PLANNER_RESULT;

/**
 * PLANNER_INPUT
 *
 * Contiene la información que debe pasarse
 * a los respectivos algoritmos de planificación
 *
 * La estructura es similar a PLANNER_RESULT, pero
 * con solamente los tiempos de llegada / servicicio
 * el cuanto, el número de procesos, y por último
 * 'idle' indica el tiempo al principio del algoritmo
 * en que el procesador está a la espera de que llegue
 * el primer proceso
 */
struct PLANNER_INPUT_s
{
	struct
	{
		int t_llegada;
		int t_servicio;
	} times[MAX_PROCESS];

	int quant;
	int num;
	int idle;
};

typedef struct PLANNER_INPUT_s PLANNER_INPUT;

/**
 * Forward declarations
 */
int proccess_arguments(int argc, char* argv[]);
PLANNER_RESULT fcfs_algorithm(PLANNER_INPUT data);
char* trim(char *s, const char *trimChars);
