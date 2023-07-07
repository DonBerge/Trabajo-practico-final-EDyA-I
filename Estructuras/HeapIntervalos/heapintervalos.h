#ifndef __HEAP_INTERVALOS_H__
#define __HEAP_INTERVALOS_H__
typedef struct _Intervalo Intervalo;

typedef struct _HeapIntervalos HeapIntervalos;

Intervalo intervalo_crear(int inicio,int final);
HeapIntervalos heap_intervalos_crear();
void heap_intervalos_destruir(HeapIntervalos);
int heap_intervalos_vacio(HeapIntervalos);
HeapIntervalos heap_intervalos_insertar(HeapIntervalos, Intervalo);
Intervalo heap_intervalos_obtener_primero(HeapIntervalos);
HeapIntervalos heap_intervalos_eliminar_primero(HeapIntervalos);
#endif