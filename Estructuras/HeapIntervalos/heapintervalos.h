#ifndef __HEAP_INTERVALOS_H__
#define __HEAP_INTERVALOS_H__
// Una estrutura de tipo heap binario que permite almacenar intervalos enteros
// El elemento que permanece en la raiz es el que tiene menor coordenada entera
// y mayor longitud

// Estructura que representa al intervalo, junto a una funciones para crearlos
// y compararlos
typedef struct _Intervalo Intervalo;
Intervalo intervalo_crear(int inicio,int final);
int intervalo_comparar(Intervalo, Intervalo);

// Estructura que representa al heap
typedef struct _HeapIntervalos HeapIntervalos;

// Crea un heap de intervalos
HeapIntervalos heap_intervalos_crear();

// Libera la memoria ocupada por el heap de intervalos
void heap_intervalos_destruir(HeapIntervalos);

// Funcion que permite determinar si un heap de intervalos esta vacio
int heap_intervalos_vacio(HeapIntervalos);

// Inserta un intervalo dado en el heap
HeapIntervalos heap_intervalos_insertar(HeapIntervalos, Intervalo);

// Devuelve el intervalo con menor coordenada inicial y mayor largo
Intervalo heap_intervalos_obtener_primero(HeapIntervalos);

// Elimina el intervalo con menor coordenada inicial y mayor largo
HeapIntervalos heap_intervalos_eliminar_primero(HeapIntervalos);

// Mueve todos los intervalos del heap una cierta cantidad de pasos hacia la izquierda
HeapIntervalos heap_intervalos_sustraer(HeapIntervalos, int);
#endif