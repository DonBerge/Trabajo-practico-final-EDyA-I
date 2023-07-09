#ifndef __BOLSA_INTERVALOS_H__
#define __BOLSA_INTERVALOS_H__
// Una estrutura de tipo heap binario que permite almacenar intervalos enteros
// El elemento que permanece en la raiz es el que tiene menor coordenada entera
// y mayor longitud

// Estructura que representa al intervalo, junto a una funciones para crearlos
// y compararlos
typedef struct {
    int inicio;
    int final;
} Intervalo;

Intervalo intervalo_crear(int inicio,int final);
int intervalo_comparar(Intervalo, Intervalo);

// Estructura que representa al heap
typedef struct _BolsaIntervalos* BolsaIntervalos;

// Crea un heap de intervalos
BolsaIntervalos bolsa_intervalos_crear();

// Libera la memoria ocupada por el heap de intervalos
void bolsa_intervalos_destruir(BolsaIntervalos);

// Funcion que permite determinar si un heap de intervalos esta vacio
int bolsa_intervalos_vacia(BolsaIntervalos);

// Inserta un intervalo dado en el heap
BolsaIntervalos bolsa_intervalos_insertar(BolsaIntervalos, Intervalo);

// Devuelve el intervalo con menor coordenada inicial y mayor largo
Intervalo bolsa_intervalos_obtener_primero(BolsaIntervalos);

// Elimina el intervalo con menor coordenada inicial y mayor largo
BolsaIntervalos bolsa_intervalos_eliminar_primero(BolsaIntervalos);

int bolsa_intervalos_size(BolsaIntervalos);

#endif