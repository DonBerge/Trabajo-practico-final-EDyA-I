#include "heapintervalos.h"
#include <stdio.h>

int main()
{
    int izq[] = {0,0,1};
    int der[] = {7,5,4};
    int n = sizeof(izq) / sizeof(int);
    HeapIntervalos l = heap_intervalos_crear();
    /*printf("%d\n",intervalo_comparar(intervalo_crear(0,7),intervalo_crear(0,19)));
    for (int i = 0; i < n; i++)
    {
        l = heap_intervalos_insertar(l, intervalo_crear(izq[i], der[i]));
        printf("(%d %d): ",heap_intervalos_obtener_primero(l).inicio, heap_intervalos_obtener_primero(l).final);
        for (int j = 0; j < l.ultimo; j++)
        {
            Intervalo a = l.array[j];
            printf("(%d %d) ", a.inicio, a.final);
        }
        printf("\n");
    }
    printf("\n\n\n");
    for (int i = 0; i < n; i++)
    {
        l = heap_intervalos_eliminar_primero(l);
        printf("(%d %d): ",heap_intervalos_obtener_primero(l).inicio, heap_intervalos_obtener_primero(l).final);
        for (int j = 0; j < heap_intervalos_size(i); j++)
        {
            Intervalo a = l.array[j];
            printf("(%d %d) ", a.inicio, a.final);
        }
        printf("\n");
    }*/
    heap_intervalos_destruir(l);
    return 0;
}