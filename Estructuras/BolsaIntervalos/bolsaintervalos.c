#include "bolsaintervalos.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// Implementa la bolsa de intervalos usando una tabla hash de pilas, la tabla hash implementa la tecnica de
// encadenamiento

// Los intervalos a insertar se hashean por su primera coordenada y luego son insertados en la pila que ocupa
// su casilla hash

Intervalo intervalo_crear(int i, int f)
{
    Intervalo x;
    x.inicio = i;
    x.final = f;
    return x;
}

struct _PilaNode
{
    Intervalo dato;
    struct _PilaNode *sig;
};

typedef struct _PilaNode *Pila;

Pila pila_crear()
{
    return NULL;
}

Pila pila_push(Pila p, Intervalo inter)
{
    struct _PilaNode *newnode = (struct _PilaNode *)malloc(sizeof(struct _PilaNode));
    newnode->dato = inter;
    newnode->sig = p;
    return newnode;
}

int pila_vacia(Pila p)
{
    return p == NULL;
}

Pila pila_pop(Pila p)
{
    if (p != NULL)
    {
        Pila newpila = p->sig;
        free(p);
        p = newpila;
    }
    return p;
}

void pila_destruir(Pila p)
{
    while (!pila_vacia(p))
        p = pila_pop(p);
}

Intervalo pila_top(Pila p)
{
    assert(p != NULL);
    return p->dato;
}

typedef struct _CasillaHash
{
    int key;
    Pila pila;
    struct _CasillaHash *sig;
} CasillaHash;

struct _BolsaIntervalos
{
    unsigned capacidad;
    unsigned size;

    // Guarda el menor valor de inicio existente entre todos los intervalos
    int menorInicio;

    CasillaHash **casillas;
};

BolsaIntervalos bolsa_intervalos_crear(int capacidad)
{
    BolsaIntervalos b = (BolsaIntervalos)malloc(sizeof(struct _BolsaIntervalos));
    b->capacidad = capacidad;
    b->size = 0;
    // Valor centinela
    b->menorInicio = -1;
    b->casillas = (CasillaHash **)malloc(sizeof(CasillaHash *) * b->capacidad);
    for(unsigned i=0;i<b->capacidad;i++)
        b->casillas[i] = NULL;
    return b;
}

void __bolsa_intervalos_destruir_lista_casillas(CasillaHash *inicio)
{
    if (inicio != NULL)
    {
        CasillaHash *sig = inicio->sig;
        pila_destruir(inicio->pila);
        free(inicio);
        __bolsa_intervalos_destruir_lista_casillas(sig);
    }
}

void bolsa_intervalos_destruir(BolsaIntervalos b)
{
    if (b != NULL)
    {
        for (unsigned i = 0; i < b->capacidad; i++)
            __bolsa_intervalos_destruir_lista_casillas(b->casillas[i]);
        free(b->casillas);
        free(b);
        b = NULL;
    }
}

int bolsa_intervalos_vacia(BolsaIntervalos b)
{
    return b->size == 0;
}

BolsaIntervalos bolsa_intervalos_insertar(BolsaIntervalos b, Intervalo inter)
{
    unsigned idx = inter.inicio % b->capacidad;
    CasillaHash *casilla_a_insertar = b->casillas[idx];

    b->menorInicio = (b->size > 0 && b->menorInicio < inter.inicio) ? b->menorInicio : inter.inicio;

    while (casilla_a_insertar != NULL && casilla_a_insertar->key != inter.inicio)
        casilla_a_insertar = casilla_a_insertar->sig;
    
    if (casilla_a_insertar == NULL)
    {
        // No existe una casilla hash asociada al elemento asi que creo una nueva
        // y la meto al inicio de la lista de casillas hash
        casilla_a_insertar = (CasillaHash *)malloc(sizeof(CasillaHash));
        casilla_a_insertar->key = inter.inicio;
        casilla_a_insertar->sig = b->casillas[idx];
        casilla_a_insertar->pila = pila_push(pila_crear(), inter);
        b->casillas[idx] = casilla_a_insertar;
    }
    else
        // Encontre la casilla hash asociada al elemento asi que lo voy a insertar
        // en dicha posicion
        casilla_a_insertar->pila = pila_push(casilla_a_insertar->pila, inter);

    

    b->size++;
    return b;
}

Intervalo bolsa_intervalos_obtener_primero(BolsaIntervalos b)
{
    CasillaHash *casilla_actual = b->casillas[b->menorInicio % b->capacidad];
    while (casilla_actual != NULL && casilla_actual->key != b->menorInicio)
        casilla_actual = casilla_actual->sig;
    assert(casilla_actual != NULL);
    return pila_top(casilla_actual->pila);
}

BolsaIntervalos bolsa_intervalos_eliminar_primero(BolsaIntervalos b)
{
    CasillaHash *casilla_actual = b->casillas[b->menorInicio % b->capacidad];

    // Me va a servir para poder eliminar la casilla hash a posteriori
    CasillaHash *casilla_previa = NULL;

    while (casilla_actual != NULL && casilla_actual->key != b->menorInicio)
    {
        casilla_previa = casilla_actual;
        casilla_actual = casilla_actual->sig;
    }
    casilla_actual->pila = pila_pop(casilla_actual->pila);

    b->size--;

    if (pila_vacia(casilla_actual->pila))
    {
        // Elimino la casilla hash
        if (casilla_previa == NULL)
        {
            // La casilla que voy a eliminar es la primera casilla de la lista
            b->casillas[b->menorInicio % b->capacidad] = NULL;
            free(casilla_actual);
        }
        else
        {
            // La casilla que voy a eliminar es una casilla intermedia
            casilla_previa->sig = casilla_actual->sig;
            free(casilla_actual);
        }

        // Si quedan elementos en la tabla, hay que actualizar el menor inicio
        if (b->size != 0)
        {
            int nuevoMenorInicio = INT_MAX-1;

            for (unsigned idx = 0; idx < b->capacidad; idx++)
                for(CasillaHash* casillaI = b->casillas[idx]; casillaI != NULL; casillaI = casillaI->sig)
                    nuevoMenorInicio = (nuevoMenorInicio < casillaI->key) ? nuevoMenorInicio : casillaI->key;
            
            b->menorInicio = nuevoMenorInicio;
        }
        else
        {
            // Caso contrario lo devuelvo al valor centinela
            b->menorInicio = -1;
        }
    }
    return b;
}