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
    node->dato = NULL;
    c.copy(&(node->dato),dato);
    if (c.size == 0)
    {
        node->sig = node;
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
        ColaNode *nodeToDestroy = c.front;
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
        c.destroy(nodeToDestroy->dato);
        free(nodeToDestroy);
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

void cola_destroy(Cola c)
{
    while(!cola_empty(c))
        c = cola_pop(c);
}

#endif