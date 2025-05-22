/* Copyright (C) 2022 Raul Torres raultorrescarvajal@gmail.com */
#include <stdio.h>
#include <omp.h>  // Cabecera que permite enlazar las funciones del runtime de OpenMP

int main(){
    /* usa tareas para calcular C como la suma de los vectores A y B,
       y luego suma todos los elementos de C.
       (- establece un número mínimo de 3 iteraciones del bucle 
          por cada tarea generada.
        - imprime en pantalla el número de hilo con cada resultado) */
    
    // Variables que pueden accederse desde cualquier hilo
    int A[16]= {0 , 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15};
    int B[16]= {15,14,13,12,11,10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
    int C[16]= {};
    
    // Variable que acumula el valor de la operación + en cada iteración
    int total=0;
    
    // Crear región paralela para generar tareas
    #pragma omp parallel
    {
        // Solo un hilo genera las tareas
        #pragma omp single
        {
            // Usar directiva taskloop con grainsize para crear tareas
            // con mínimo 3 iteraciones por tarea
            #pragma omp taskloop grainsize(3)
            for(int i=0; i<16; i++){
                C[i] = A[i] + B[i];
                printf("Hilo %d: C[%d]=%d\n", omp_get_thread_num(), i, C[i]);
            }
            
            // Esperar a que todas las tareas terminen antes de continuar
            #pragma omp taskwait
            
            // Calcular el total después de que todas las tareas hayan terminado
            for(int i=0; i<16; i++){
                total += C[i];
            }
            printf("Hilo %d: total=%d\n", omp_get_thread_num(), total);
        }
    }
    
    return 0;
}
