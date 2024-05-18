# Taller-de-evaluacion-de-rendimiento
En este repositorio podras encontrar varios archivos, lee este texto para saber que contiene cada archivo y como puedes realizar tus propias pruebas.

C:

En los archivos .c se encuentra el codigo principal del programa en mm_clasico.c encontraras la implementacion del algortimo clasico de multiplicacion de matrices, en mm_transpuesta.c encontraras la implemantacion del algortimo transpuesta de multiplicacion de matrices y en el main.c encontraras que puedes usar cualquiera de los dos algoritmos y la creacion y uso de hilos.

H:

En el archivo .h se encuentran las definiciones de las funciones que se van a usar en los archivos .c. Aqui basicamente se esta creando una libreria propia.

PEARL:

En el archivo .pl se encuentra un script que sirve para ejecutar todos los casos de prueba que se quieren realizar y guardar los reultados en archivos .dat

CSV:

Existe cuatro archivos .csv donde podras encontrar los resultados obtenidos de la ejecucion del programa usando diferentes cantidades de hilos y diferentes dimensiones de la matriz. Si quieres realizar comparaciones con otros sistemas de computo los sistemas usados fueron: Linux, Ubuntu con 8 cores y Windows 10 con 4 cores.

Para hacer tus propias puedes realizar cambios al script pearl para cambiar la cantidad de hilos o la dimension de la matriz, tambien puedes implementar nuevos algortimos de multiplicacion de matrices y añadirlos a la libreria para usarlos y ver sus resultados.
