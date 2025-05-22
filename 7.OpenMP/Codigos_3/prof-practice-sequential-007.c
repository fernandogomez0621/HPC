/* Copyright (C) 2022 Raul Torres raultorrescarvajal@gmail.com */
#include <stdio.h>
#include <omp.h>  // Cabecera que permite enlazar las funciones del runtime de OpenMP

int max(int a, int b){
    return a >= b ? a : b;
}

int main(){
    /* determina el número máximo comparando entre pares de números 
       (imprime en pantalla el número de hilo con cada resultado) */
    
    // Variables de solo lectura
    int a=40, b=20, c=30, d=50;
    int w=25, x=73, y=99, z=5;
    
    // Variables que se escriben y se leen una sola vez 
    int max_0=0, max_1=0, max_2=0, max_3=0, max_4=0, max_5=0;
    
    // Variable que solo se escribe una sola vez
    int final_max=0;
    
    // Crear región paralela para generar tareas
    #pragma omp parallel
    {
        #pragma omp single
        {
            // Primer grupo: 2 operaciones independientes
            #pragma omp task depend(out: max_0)
            {
                max_0 = max(a, b); 
                printf("Hilo %d: max_0=%d\n", omp_get_thread_num(), max_0);
            }
            
            #pragma omp task depend(out: max_1)
            {
                max_1 = max(c, d); 
                printf("Hilo %d: max_1=%d\n", omp_get_thread_num(), max_1);
            }
            
            // Esta operación depende de max_0 y max_1
            #pragma omp task depend(in: max_0, max_1) depend(out: max_4)
            {
                max_4 = max(max_0, max_1); 
                printf("Hilo %d: max_4=%d\n", omp_get_thread_num(), max_4);
            }
            
            // Segundo grupo: 2 operaciones independientes entre sí y del primer grupo
            #pragma omp task depend(out: max_2)
            {
                max_2 = max(w, x); 
                printf("Hilo %d: max_2=%d\n", omp_get_thread_num(), max_2);
            }
            
            #pragma omp task depend(out: max_3)
            {
                max_3 = max(y, z); 
                printf("Hilo %d: max_3=%d\n", omp_get_thread_num(), max_3);
            }
            
            // Esta operación depende de max_2 y max_3
            #pragma omp task depend(in: max_2, max_3) depend(out: max_5)
            {
                max_5 = max(max_2, max_3); 
                printf("Hilo %d: max_5=%d\n", omp_get_thread_num(), max_5);
            }
            
            // Operación final que depende de max_4 y max_5
            #pragma omp task depend(in: max_4, max_5)
            {
                final_max = max(max_4, max_5); 
                printf("Hilo %d: final_max_6=%d\n", omp_get_thread_num(), final_max);
            }
        }
    }
    
    return 0;
}
