#ifndef __HEAP_INTERVALOS_C__
#define __HEAP_INTERVALOS_C__
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
    HeapIntervalos h = (struct _HeapIntervalos*)malloc(sizeof(struct _HeapIntervalos));
    h->array = NULL;
    h->ultimo = 0;
    h->capacidad = 0;
    return h;
};

void heap_intervalos_destruir(HeapIntervalos heap)
{
    free(heap->array);
    free(heap);
}

int heap_intervalos_vacio(HeapIntervalos heap)
{
    return heap->ultimo == 0;
}

// Hace trepar un nodo del heap si no cumple la invariante heap, cambiandolo con sus ancestros
// que tienen menor prioridad
void heap_intervalos_trepar_elemento(Intervalo* array, int index)
{
    int trepo = 1;
    for(int i = index; i>0 && trepo; i = (i-1)/2)
        // Si el nodo actual tiene mayor prioridad que su padre, entonces los intercambio
        if(intervalo_comparar(array[(i-1)/2],array[i]) == 1)
        {
            Intervalo temp = array[(i-1)/2];
            array[(i-1)/2]=array[i];
            array[i]=temp;
        }
        else
            trepo = 0;
}

Intervalo heap_intervalos_obtener_primero(HeapIntervalos heap)
{
    return heap->array[0];
}

// Hunde un nodo en el heap si no cumple la invariante heap, cambiandolo con los hijos que tienen mayor
// prioridad que el
HeapIntervalos heap_intervalos_hundir_elemento(HeapIntervalos h, int index)
{
    int seHundio = 1;
    for(int i=index; i<h->ultimo && seHundio; )
    {
        // Comparo el nodo actual con sus dos hijos y obtengo el mas grande
        int largest = i;
        if(i*2+1 < h->ultimo && intervalo_comparar(h->array[largest],h->array[i*2+1])==1)
            largest = i*2+1;
        if(i*2+2 < h->ultimo && intervalo_comparar(h->array[largest],h->array[i*2+2])==1)
            largest = i*2+2;

        // Si el nodo actual es el mas grande entonces no hay que hundirlo
        if(largest == i)
            seHundio = 0;
        else
        {
            // En caso contrario lo cambio con su hijo mas grande
            Intervalo temp = h->array[largest];
            h->array[largest] = h->array[i];
            h->array[i] = temp;
            i=largest;
        }
    }
    return h;
}

HeapIntervalos heap_intervalos_insertar(HeapIntervalos heap, Intervalo inter)
{
    // Redimensiono el heap si es necesario
    if(heap->ultimo == heap->capacidad)
    {
        heap->capacidad = (heap->capacidad == 0) ? 1 : heap->capacidad * 2;
        heap->array = realloc(heap->array, sizeof(Intervalo)*heap->capacidad);
    }

    // Meto el nuevo intervalo en el fondo del heap y lo hago trepar
    heap->array[heap->ultimo] = inter;
    heap_intervalos_trepar_elemento(heap->array,heap->ultimo);
    heap->ultimo++;
    return heap;
}

HeapIntervalos heap_intervalos_eliminar_primero(HeapIntervalos heap)
{
    if(heap->ultimo != 0)
    {
        // El intervalo a eliminar es el que esta al inicio del heap, para eliminarlo
        // lo sobreescribo con el ultimo intervalo del heap y luego lo hundo
        heap->array[0] = heap->array[--heap->ultimo];
        heap = heap_intervalos_hundir_elemento(heap,0);
    }
    return heap;
}

HeapIntervalos heap_intervalos_sustraer(HeapIntervalos heap, int cant)
{
    // Recorro cada intervalo del heap y le sustraigo la cantidad pasada
    for(int i=0;cant != 0 && i<heap->ultimo;i++)
    {
        heap->array[i].inicio-=cant;
        heap->array[i].final-=cant;
    }
    return heap;
}

int heap_intervalos_size(HeapIntervalos heap)
{
    return heap->ultimo;
}

#endif