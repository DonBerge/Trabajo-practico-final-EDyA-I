#include "heapintervalos.h"
#include <stdlib.h>

struct _Intervalo {
    int inicio;
    int final;
};

struct _HeapIntervalos {
    Intervalo* array;
    int ultimo;
    int capacidad;
};

// Un intervalo a tiene mayor prioridad que otro intervalo b si
// su coordenada de inicio es menor o si tienen las mismas coordenadas
// de inicio y la longitud de a es mayor que la de b
int intervalo_comparar(Intervalo a, Intervalo b)
{
    if(a.inicio < b.inicio)
        return -1;
    if(a.inicio == b.inicio)
    {
        if(a.final > b.final)
            return -1;
        if(a.final == b.final)
            return 0;
        return 1;
    }
    return 1;
}

Intervalo intervalo_crear(int inicio, int final)
{
    Intervalo inter;
    inter.inicio = inicio;
    inter.final = final;
    return inter;
}

HeapIntervalos heap_intervalos_crear()
{
    HeapIntervalos h;
    h.array = NULL;
    h.ultimo = 0;
    h.capacidad = 0;
    return h;
};

void heap_intervalos_destruir(HeapIntervalos heap)
{
    free(heap.array);
}

int heap_intervalos_vacio(HeapIntervalos heap)
{
    return heap.ultimo == 0;
}

void heap_intervalos_trepar_elemento(Intervalo* array, int index)
{
    int trepo = 1;
    for(int i = index; i>0 && trepo; i = (i-1)/2)
        if(intervalo_comparar(array[(i-1)/2],array[i]) == 1)/*compare(array[(i-1)/2],array[i]) == -1*/
        {
            // Hago trepar el nodo
            Intervalo temp = array[(i-1)/2];
            array[(i-1)/2]=array[i];
            array[i]=temp;
        }
        else
            trepo = 0;
}

Intervalo heap_intervalos_obtener_primero(HeapIntervalos heap)
{
    return heap.array[0];
}

HeapIntervalos heap_intervalos_hundir_elemento(HeapIntervalos h, int index)
{
    int seHundio = 1;
    for(int i=index; i<h.ultimo && seHundio; )
    {
        int largest = i;
        if(i*2+1 < h.ultimo && intervalo_comparar(h.array[largest],h.array[i*2+1])==1)
            largest = i*2+1;
        if(i*2+2 < h.ultimo && intervalo_comparar(h.array[largest],h.array[i*2+2])==1)
            largest = i*2+2;
        if(largest == i)
            seHundio = 0;
        else
        {
            Intervalo temp = h.array[largest];
            h.array[largest] = h.array[i];
            h.array[i] = temp;
            i=largest;
        }
    }
    return h;
}

HeapIntervalos heap_intervalos_insertar(HeapIntervalos heap, Intervalo inter)
{
    if(heap.ultimo == heap.capacidad)
    {
        heap.capacidad = (heap.capacidad == 0) ? 1 : heap.capacidad * 2;
        heap.array = realloc(heap.array, sizeof(Intervalo)*heap.capacidad);
    }
    heap.array[heap.ultimo] = inter;
    heap_intervalos_trepar_elemento(heap.array,heap.ultimo);
    heap.ultimo++;
    return heap;
}

HeapIntervalos heap_intervalos_eliminar_primero(HeapIntervalos heap)
{
    if(heap.ultimo != 0)
    {
        heap.array[0] = heap.array[--heap.ultimo];
        heap = heap_intervalos_hundir_elemento(heap,0);
    }
    return heap;
}