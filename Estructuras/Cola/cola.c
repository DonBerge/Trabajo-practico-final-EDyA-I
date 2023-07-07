#ifndef __COLA__C__
#define __COLA__C__
#include "cola.h"
#include <stdlib.h>

typedef struct _ColaNode
{
    void *dato;
    struct _ColaNode *sig;
} ColaNode;

struct _Cola
{
    ColaNode *front;
    ColaNode *back;
    FuncionCopia copy;
    FuncionDestructora destroy;
    int size;
};

Cola cola_crear(FuncionCopia copy, FuncionDestructora destroy)
{
    Cola c;
    c.front = NULL;
    c.back = NULL;
    c.copy = copy;
    c.destroy = destroy;
    c.size = 0;
    return c;
}

int cola_size(Cola c)
{
    return c.size;
}

int cola_empty(Cola c)
{
    return c.size == 0;
}

Cola cola_push(Cola c, void *dato)
{
    ColaNode *node = (ColaNode *)malloc(sizeof(ColaNode));
    node->dato = c.copy(dato);
    node->sig = NULL;
    if (c.size == 0)
    {
        c.front = node;
        c.back = node;
    }
    else
    {
        c.back->sig = node;
        c.back = node;
    }
    c.size++;
    return c;
}

Cola cola_pop(Cola c)
{
    if (c.size != 0)
    {
        ColaNode *node_to_destroy = c.front;
        if (c.size == 1)
        {
            c.front = NULL;
            c.back = NULL;
        }
        else
        {
            c.front = c.front->sig;
        }
        c.size--;
        c.destroy(node_to_destroy->dato);
        free(node_to_destroy);
    }
    return c;
}

void *cola_front(Cola c)
{
    return c.front->dato;
}

void *cola_back(Cola c)
{
    return c.back->dato;
}

void cola_visitar(Cola c, FuncionVisitante visit)
{
    for (ColaNode *i = c.front; i != NULL; i = i->sig)
        visit(i->dato);
}

void cola_destroy(Cola c)
{
    while (c.size)
        c = cola_pop(c);
}

#endif