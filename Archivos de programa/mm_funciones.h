/***************************************************************
	 Pontificia Universidad Javeriana
	 Autor: J. Corredor
	 Fecha: Febrero 2024
	 Materia: Sistemas Operativos
	 Tema: Taller de Evaluación de Rendimiento
	 Fichero: fuente de multiplicación de matrices NxN por hilos.
	 Objetivo: Evaluar el tiempo de ejecución del 
						 algoritmo clásico de multiplicación de matrices.
						 Se implementa con la Biblioteca POSIX Pthreads
***************************************************************/
#ifndef MM_CLASICO_H
#define MM_CLASICO_H

#include <pthread.h>
#include <time.h>
#include <sys/time.h>

#define DATA_SIZE (1024*1024*64*3) 

// Declaración de un mutex para sincronización entre hilos
extern pthread_mutex_t MM_mutex;
// Declaración de punteros globales para las matrices
extern double *mA, *mB, *mC;


// Estructura que contiene los parámetros que se pasan a cada hilo
struct parametros{
		int nH;
		int idH;
		int N;
};

// Declaración de variables para medir el tiempo de ejecución
extern struct timeval start, stop; 

//Funcion encargada de llenar la matriz
void llenar_matriz(int SZ);

//Funcion encargada de imprimir la matriz
void print_matrix(int sz, double *matriz);

//Inicia el contador
void inicial_tiempo();

//Finaliza el contador
void final_tiempo();

//Funcion que ejecuta el hilo
void *mult_thread(void *variables);

#endif
