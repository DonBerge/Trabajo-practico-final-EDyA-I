#include "dlist.h"
#include <stdlib.h>
#include <assert.h>

typedef struct _DNodo
{
    void *dato;
    struct _DNodo *prev;
    struct _DNodo *sig;
} DNodo;

struct _DList
{
    DNodo *inicio;
    DNodo *final;
    FuncionCopia copy;
    FuncionDestructora destroy;
};

DList dlist_crear(FuncionCopia copy, FuncionDestructora destroy)
{
    DList d;
    d.inicio = d.final = NULL;
    d.copy = copy;
    d.destroy = destroy;
    return d;
}

DNodo *dlist_crear_nodo(void *dato, FuncionCopia copy)
{
    DNodo *newnode = (DNodo *)malloc(sizeof(DNodo));
    void* a = copy(dato);
    newnode->dato = a;
    newnode->prev = NULL;
    newnode->sig = NULL;
    return newnode;
}

void dlist_destruir_nodo(DNodo* nodo, FuncionDestructora destroy)
{
    destroy(nodo->dato);
    free(nodo);
}

int dlist_vacia(DList lista)
{
    return lista.inicio == lista.final && lista.inicio == NULL;
}

DList dlist_insertar_inicio(DList lista, void *dato)
{
    DNodo *nodo = dlist_crear_nodo(dato, lista.copy);
    if (dlist_vacia(lista))
    {
        lista.inicio = nodo;
        lista.final = nodo;
    }
    else
    {
        nodo->sig = lista.inicio;
        lista.inicio->prev = nodo;
        lista.inicio = nodo;
    }
    return lista;
}

DList dlist_insertar_final(DList lista, void *dato)
{
    DNodo *nodo = dlist_crear_nodo(dato, lista.copy);
    if (dlist_vacia(lista))
    {
        lista.inicio = nodo;
        lista.final = nodo;
    }
    else
    {
        nodo->prev = lista.final;
        lista.final->sig = nodo;
        lista.final = nodo;
    }
    return lista;
}

DList dlist_eliminar_inicio(DList lista)
{
    DNodo* nodo_a_destruir = lista.inicio;
    if(lista.inicio == lista.final)
        lista.inicio = lista.final = NULL;
    else
    {
        lista.inicio = lista.inicio->sig;
        lista.inicio->prev = NULL;
    }
    dlist_destruir_nodo(nodo_a_destruir, lista.destroy);
    return lista;
}

DList dlist_eliminar_final(DList lista)
{
    DNodo* nodo_a_destruir = lista.final;
    if(lista.inicio == lista.final)
        lista.inicio = lista.final = NULL;
    else
    {
        lista.final = lista.final->prev;
        lista.final->sig = NULL;
    }
    dlist_destruir_nodo(nodo_a_destruir, lista.destroy);
    return lista;
}

void* dlist_elemento_inicio(DList lista)
{
    assert(!dlist_vacia(lista));
    return lista.inicio->dato;
}

void* dlist_elemento_final(DList lista)
{
    assert(!dlist_vacia(lista));
    return lista.final->dato;
}

void dlist_destruir(DList lista)
{
    while(!dlist_vacia(lista))
        lista = dlist_eliminar_inicio(lista);
}