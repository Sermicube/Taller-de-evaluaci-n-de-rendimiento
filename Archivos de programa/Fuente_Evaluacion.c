/**************************************************************
		Pontificia Universidad Javeriana
	Autor: J. Corredor
	Fecha: Febrero 2024
	Materia: Sistemas Operativos
	Tema: Taller de Evaluación de Rendimiento
	Fichero: fuente de multiplicación de matrices NxN por hilos.
	Objetivo: Evaluar el tiempo de ejecución del 
		      algoritmo clásico de multiplicación de matrices.
			  Se implementa con la Biblioteca POSIX Pthreads
****************************************************************/

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

//Se define el tamaño de la variable DATASIZE
#define DATA_SIZE (1024*1024*64*3) 

//Se define un mutex, se reserva un espacio en la memoria y se declaran otras variables globales
pthread_mutex_t MM_mutex;
static double MEM_CHUNK[DATA_SIZE];
double *mA, *mB, *mC;

//Se crea una estructura que contiene el numero de hilos, sus id y el tamaño de la matriz
struct parametros{
	int nH;
	int idH;
	int N;
};

struct timeval start, stop;

//Se crea la funcion para llenar la matriz
void llenar_matriz(int SZ){ 
	srand48(time(NULL));
	//Mientras i sea menor al tamaño de la matriz, se guarda el valor en la casilla
	for(int i = 0; i < SZ*SZ; i++){
			mA[i] = 1.1*i; //drand48(); 
			mB[i] = 2.2*i; //drand48();
			mC[i] = 0; 
		}	
}

void print_matrix(int sz, double *matriz){
	if(sz < 12){
    		for(int i = 0; i < sz*sz; i++){
     				if(i%sz==0) printf("\n");
            		printf(" %.3f ", matriz[i]);
			}	
	}
    printf("\n>-------------------->\n");
}

//Funcion para iniciar un contador de tiempo
void inicial_tiempo(){
	gettimeofday(&start, NULL);
}

//Funcion para finalizar un contador de tiempo
void final_tiempo(){
	gettimeofday(&stop, NULL);
	stop.tv_sec -= start.tv_sec;
	printf("\n:-> %9.0f µs\n", (double) (stop.tv_sec*1000000 + stop.tv_usec));
}

//Función que multiplica matrices en varios hilos
void *mult_thread(void *variables){
	struct parametros *data = (struct parametros *)variables;

	//Se extraen los datos de la estructura
	int idH = data->idH;
	int nH  = data->nH;
	int N   = data->N;
	int ini = (N/nH)*idH;
	int fin = (N/nH)*(idH+1);

	// Iteración sobre la sección de la matriz a procesar por el hilo actual
    for (int i = ini; i < fin; i++){
        for (int j = 0; j < N; j++){
			double *pA, *pB, sumaTemp = 0.0;
			pA = mA + (i*N); 
			pB = mB + j;
            for (int k = 0; k < N; k++, pA++, pB+=N){
				sumaTemp += (*pA * *pB);
			}
			mC[i*N+j] = sumaTemp;
		}
	}

	pthread_mutex_lock (&MM_mutex);// Se bloquea el mutex para evitar condiciones de carrera
	pthread_mutex_unlock (&MM_mutex); // Se desbloquea el mutex al salir de la sección crítica
	pthread_exit(NULL);
}

int main(int argc, char *argv[]){
	// Se verifica si se ingresaron suficientes argumentos en la línea de comandos
	if (argc < 2){
		printf("Ingreso de argumentos \n $./ejecutable tamMatriz numHilos\n");
		return -1;	
	}
	// Se obtienen el tamaño de la matriz y el número de hilos desde la línea de comandos
    int SZ = atoi(argv[1]); 
    int n_threads = atoi(argv[2]); 

    pthread_t p[n_threads];
    pthread_attr_t atrMM;

	// Se reserva memoria para las matrices A, B y C
	mA = MEM_CHUNK;
	mB = mA + SZ*SZ;
	mC = mB + SZ*SZ;

	// Se llenan las matrices A y B
	llenar_matriz(SZ);
	print_matrix(SZ, mA);
	print_matrix(SZ, mB);

	// Se inicializa el tiempo y se inicializa el mutex para proteger la matriz C
	inicial_tiempo();
	pthread_mutex_init(&MM_mutex, NULL);
	pthread_attr_init(&atrMM);
	pthread_attr_setdetachstate(&atrMM, PTHREAD_CREATE_JOINABLE);

	// Se crean los hilos para realizar la multiplicación de matrices
    for (int j=0; j<n_threads; j++){
			// Se asignan los parámetros para cada hilo
		struct parametros *datos = (struct parametros *) malloc(sizeof(struct parametros)); 
		datos->idH = j;
		datos->nH  = n_threads;
		datos->N   = SZ;
        pthread_create(&p[j],&atrMM,mult_thread,(void *)datos);
	}
	// Se espera a que todos los hilos terminen
    for (int j=0; j<n_threads; j++)
        pthread_join(p[j],NULL);
	// Se finaliza el tiempo de ejecución
	final_tiempo();

	// Se imprime la matriz resultado
	print_matrix(SZ, mC);

	// Se destruyen los atributos del hilo y el mutex
	pthread_attr_destroy(&atrMM);
	pthread_mutex_destroy(&MM_mutex);
	pthread_exit (NULL);
}
