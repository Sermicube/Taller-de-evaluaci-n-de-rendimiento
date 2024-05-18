#!/usr/bin/perl
#**************************************************************
#         		Pontificia Universidad Javeriana
#     Autor: J. Corredor
#     Fecha: Febrero 2024
#     Materia: Sistemas Operativos
#     Tema: Taller de Evaluación de Rendimiento
#     Fichero: script automatización ejecución por lotes 
#****************************************************************/

# Definir la ruta actual y eliminar cualquier carácter de nueva línea al final
$Path = `pwd`;
chomp($Path);

# Nombre del ejecutable que se va a utilizar
$Nombre_Ejecutable = "MM_ejecutable";
# Tamaños de matriz para los cuales se ejecutará el programa
@Size_Matriz = ("100","200");
# Números de hilos para los cuales se ejecutará el programa
@Num_Hilos = (1,2,3,4);
# Número de repeticiones para cada configuración
$Repeticiones = 30;

# Bucle para iterar sobre los diferentes tamaños de matriz y números de hilos
foreach $size (@Size_Matriz){
	foreach $hilo (@Num_Hilos) {
  # Construir el nombre del archivo de datos
		$file = "$Path/$Nombre_Ejecutable-".$size."-Hilos-".$hilo.".dat";
    # Bucle para ejecutar el programa y guardar los resultados en el archivo de datos
		for ($i=0; $i<$Repeticiones; $i++) {
    # Ejecutar el programa y redirigir la salida al archivo de datos
    # Comentado para imprimir la ruta del ejecutable y los argumentos en su lugar
      system("$Path/$Nombre_Ejecutable $size $hilo  >> $file");
			printf("$Path/$Nombre_Ejecutable $size $hilo \n");
		}
    # Cerrar el archivo de datos
		close($file);
  # Incrementar la variable $p
	$p=$p+1;
	}
}
