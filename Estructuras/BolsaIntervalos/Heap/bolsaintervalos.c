#ifndef __BOLSA_INTERVALOS_C__
#define __BOLSA_INTERVALOS_C__
#include "bolsaintervalos.h"
#include <stdlib.h>

struct _Intervalo {
    int inicio;
    int final;
};

struct _BolsaIntervalos {
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

BolsaIntervalos bolsa_intervalos_crear()
{
    BolsaIntervalos h = (struct _BolsaIntervalos*)malloc(sizeof(struct _BolsaIntervalos));
    h->array = NULL;
    h->ultimo = 0;
    h->capacidad = 0;
    return h;
};

void bolsa_intervalos_destruir(BolsaIntervalos heap)
{
    free(heap->array);
    free(heap);
}

int bolsa_intervalos_vacia(BolsaIntervalos heap)
{
    return heap->ultimo == 0;
}

// Hace trepar un nodo del heap si no cumple la invariante heap, cambiandolo con sus ancestros
// que tienen menor prioridad
void bolsa_intervalos_trepar_elemento(Intervalo* array, int index)
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

Intervalo bolsa_intervalos_obtener_primero(BolsaIntervalos heap)
{
    return heap->array[0];
}

// Hunde un nodo en el heap si no cumple la invariante heap, cambiandolo con los hijos que tienen mayor
// prioridad que el
BolsaIntervalos bolsa_intervalos_hundir_elemento(BolsaIntervalos h, int index)
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

BolsaIntervalos bolsa_intervalos_insertar(BolsaIntervalos heap, Intervalo inter)
{
    // Redimensiono el heap si es necesario
    if(heap->ultimo == heap->capacidad)
    {
        heap->capacidad = (heap->capacidad == 0) ? 1 : heap->capacidad * 2;
        heap->array = realloc(heap->array, sizeof(Intervalo)*heap->capacidad);
    }

    // Meto el nuevo intervalo en el fondo del heap y lo hago trepar
    heap->array[heap->ultimo] = inter;
    bolsa_intervalos_trepar_elemento(heap->array,heap->ultimo);
    heap->ultimo++;
    return heap;
}

BolsaIntervalos bolsa_intervalos_eliminar_primero(BolsaIntervalos heap)
{
    if(heap->ultimo != 0)
    {
        // El intervalo a eliminar es el que esta al inicio del heap, para eliminarlo
        // lo sobreescribo con el ultimo intervalo del heap y luego lo hundo
        heap->array[0] = heap->array[--heap->ultimo];
        heap = bolsa_intervalos_hundir_elemento(heap,0);
    }
    return heap;
}

int bolsa_intervalos_size(BolsaIntervalos heap)
{
    return heap->ultimo;
}

#endif