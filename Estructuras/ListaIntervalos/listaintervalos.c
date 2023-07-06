#include "listaintervalos.h"
#include <stdlib.h>

struct _NodoListaIntervalos
{
    Intervalo dato;
    struct _NodoListaIntervalos *prev, *next;
};

struct _ListaIntervalos
{
    NodoListaIntervalos *inicio;
    NodoListaIntervalos *final;
};

NodoListaIntervalos *lista_intervalos_crear_nodo(int inicio,int final)
{
    Intervalo x;
    x.inicio = inicio;
    x.final = final;
    NodoListaIntervalos *nuevoNodo = (NodoListaIntervalos *)malloc(sizeof(NodoListaIntervalos));
    nuevoNodo->dato = x;
    nuevoNodo->prev = nuevoNodo->next = NULL;
    return nuevoNodo;
}

void lista_intervalos_destruir_nodo(NodoListaIntervalos *nodo)
{
    free(nodo);
}

ListaIntervalos lista_intervalos_crear()
{
    ListaIntervalos l;
    l.inicio = l.final = NULL;
    return l;
}

int lista_intervalos_vacia(ListaIntervalos l)
{
    return l.inicio == NULL && l.final == NULL;
}

ListaIntervalos lista_intervalos_insertar_inicio(ListaIntervalos l, int inicio, int final)
{
    NodoListaIntervalos* nuevoNodo = lista_intervalos_crear_nodo(inicio,final);
    if(lista_intervalos_vacia(l))
        l.inicio = l.final = nuevoNodo;
    else
    {
        l.inicio->prev = nuevoNodo;
        nuevoNodo->next = l.inicio;
        l.inicio = nuevoNodo;
    }
    return l;
}

ListaIntervalos lista_intervalos_insertar_final(ListaIntervalos l, int inicio, int final)
{
    NodoListaIntervalos* nuevoNodo = lista_intervalos_crear_nodo(inicio,final);
    if(lista_intervalos_vacia(l))
        l.inicio = l.final = nuevoNodo;
    else
    {
        l.final->next = nuevoNodo;
        nuevoNodo->prev = l.final;
        l.final = nuevoNodo;
    }
    return l;
}

ListaIntervalos lista_intervalos_eliminar_inicio(ListaIntervalos l)
{
    if (!lista_intervalos_vacia(l))
    {
        if (l.inicio == l.final)
        {
            lista_intervalos_destruir_nodo(l.inicio);
            l.inicio = l.final = NULL;
        }
        else
        {
            NodoListaIntervalos *nodo_a_destruir = l.inicio;
            l.inicio = l.inicio->next;
            l.inicio->prev = NULL;
            lista_intervalos_destruir_nodo(nodo_a_destruir);
        }
    }
    return l;
}

ListaIntervalos lista_intervalos_eliminar_final(ListaIntervalos l)
{
    if (!lista_intervalos_vacia(l))
    {
        if (l.inicio == l.final)
        {
            lista_intervalos_destruir_nodo(l.final);
            l.inicio = l.final = NULL;
        }
        else
        {
            NodoListaIntervalos *nodo_a_destruir = l.final;
            l.final = l.final->prev;
            l.final->next = NULL;
            lista_intervalos_destruir_nodo(nodo_a_destruir);
        }
    }
    return l;
}

Intervalo lista_intervalos_elemento_inicio(ListaIntervalos l)
{
    return l.inicio->dato;
}

Intervalo lista_intervalos_elemento_final(ListaIntervalos l)
{
    return l.final->dato;
}

ListaIntervalos lista_intervalos_destruir(ListaIntervalos l)
{
    while(!lista_intervalos_vacia(l))
        l = lista_intervalos_eliminar_final(l);
    l.inicio = l.final = NULL;
    return l;
}