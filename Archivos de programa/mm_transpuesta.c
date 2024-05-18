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
#include "mm_funciones.h"

#define DATA_SIZE (1024*1024*64*3)

double *mA, *mB, *mC;
pthread_mutex_t MM_mutex;
struct timeval start, stop; 

// Funcion para llenar una matriz
void llenar_matriz(int SZ){ 
	srand48(time(NULL)); // Funcion usada para generar numeros aleatorios
	for(int i = 0; i < SZ*SZ; i++){
			mA[i] = 1.1*i; //drand48(); 
			mB[i] = 2.2*i; //drand48();
			mC[i] = 0; 
		}	
}

// Funcion para imprimir una matriz
void print_matrix(int sz, double *matriz){
	if(sz < 12){ // Si la matriz es menor a 12x12 se imprime
    		for(int i = 0; i < sz*sz; i++){
     				if(i%sz==0) printf("\n");
            		printf(" %.3f ", matriz[i]);
			}	
    	printf("\n>-------------------->\n");
	}
}

// Funcion para iniciar a contar el tiempo
void inicial_tiempo(){
	gettimeofday(&start, NULL); // Obtener el timepo actual
}

// Funcion para acabar de contar el tiempo transcurrido
void final_tiempo(){
	gettimeofday(&stop, NULL); // Obtener el timepo actual
	stop.tv_sec -= start.tv_sec; // Restar los segundos del tiempo stop y start
	printf("\n:-> %9.0f µs\n", (double) (stop.tv_sec*1000000 + stop.tv_usec)); // Imprimir el tiempo transcurrido
}

// Funcion para multiplicar dos matrices
void *mult_thread(void *variables){
	struct parametros *data = (struct parametros *)variables; // Se obtiene el argumento
	
	int idH = data->idH; // Se obtiene el id del hilo
	int nH  = data->nH; // Se obtiene el numero de hilos
	int N   = data->N; // Se obtiene el tamaño de la matriz
	int ini = (N/nH)*idH; // Se calcula el inicio del recorrido
	int fin = (N/nH)*(idH+1); // Se calcula el final del recorrido

    for (int i = ini; i < fin; i++){ // Recorrer las filas de ini a fin
        for (int j = 0; j < N; j++){ // Recorrer todas las columnas
			double *pA, *pB, sumaTemp = 0.0; // Declarar dos apuntadores y una suma temporal
			pA = mA + (i*N); // Al apuntador pA darle el valor de mA + el numero de fila actual
			pB = mB + (j*N); // Al apuntador pB darle el valor de mB + el numero de columna actual
            for (int k = 0; k < N; k++, pA++, pB++){ // Aumentar pA de uno en uno y pB de uno en uno
				sumaTemp += (*pA * *pB); // Calcular el resultado de la multiplicacion de fila por columna
			}
			mC[i*N+j] = sumaTemp; // Colocar el resultado de la operacion en el lugar correspondiente
		}
	}

	pthread_mutex_lock (&MM_mutex); // Bloquear el mutex
	pthread_mutex_unlock (&MM_mutex); // Desbloquear el mutex
	pthread_exit(NULL); // Acabar la funcion
}
