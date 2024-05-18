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

extern pthread_mutex_t MM_mutex;
double MEM_CHUNK[DATA_SIZE];
extern double *mA, *mB, *mC;
extern struct timeval start, stop; 

int main(int argc, char *argv[]){
	if (argc < 2){ // Verificar que la cantidad de argumentos sea correcta
		printf("Ingreso de argumentos \n $./ejecutable tamMatriz numHilos\n");
		return -1;	
	}
		int SZ = atoi(argv[1]);  // Obtener el tamaño de la matriz
		int n_threads = atoi(argv[2]);  // Obtener el numero de hilos que se va a usar

		pthread_t p[n_threads]; // Declarar un arreglo de hilos con la cantidad obtenida anteriormente
		pthread_attr_t atrMM; // Declarar un atributo de un hilo

	mA = MEM_CHUNK; // Apuntar mA al principio de la memoria reservada
	mB = mA + SZ*SZ; // Apuntar mB al espacio de memoria despues de mA
	mC = mB + SZ*SZ; // Apuntar mC al espacio de memoria despues de mB 

	llenar_matriz(SZ); // Se llegan las matrices
	print_matrix(SZ, mA); // Se imprime la matriz mA
	print_matrix(SZ, mB); // Se imprime la matriz mB

	inicial_tiempo(); // Se inicia a contar el tiempo
	pthread_mutex_init(&MM_mutex, NULL); // Se inicializa el mutex
	pthread_attr_init(&atrMM); // Se inicializa el atributo
	pthread_attr_setdetachstate(&atrMM, PTHREAD_CREATE_JOINABLE); // Se establece que el estado de desprendimiento sea PTHREAD_CREATE_JOINABLE

		// Se hace un for para crear todos los hilos
		for (int j=0; j<n_threads; j++){
		struct parametros *datos = (struct parametros *) malloc(sizeof(struct parametros)); // Se declara una variable tipo struct parametros
		datos->idH = j;
		datos->nH  = n_threads; // Se da los valores correspondientes a la estructura
		datos->N   = SZ;
				pthread_create(&p[j],&atrMM,mult_thread,(void *)datos); // Se hace pthread_create con el atributo atrMM, la funcion mult_thread y la estructura datos
	}

		for (int j=0; j<n_threads; j++)
				pthread_join(p[j],NULL); // Se espera a que todos los hilos acaben de ejecutarse
	final_tiempo(); // Se finaliza el conteo del tiempo

	print_matrix(SZ, mC); // Se imprime la matriz resultado

	pthread_attr_destroy(&atrMM); // Se destruye al atributo
	pthread_mutex_destroy(&MM_mutex); // Se destruye el mutex
	pthread_exit (NULL); // Se finaliza el main
}
