#include "map.h"
#include <stdlib.h>
// Implemento el map como un arbol binario de busqueda
struct _MapNode
{
    void *llave;
    void *valor;
    struct _MapNode *izq;
    struct _MapNode *der;
};

struct _Map
{
    struct _MapNode* raiz;
    FuncionCopiadoraLlave copy_llave;
    FuncionCopiadoraValor copy_valor;
    FuncionComparadoraLlave compare_llave;
};

Map map_crear(FuncionComparadoraLlave cmp_llave,FuncionCopiadoraLlave cpy_llave,FuncionCopiadoraValor cpy_valor)
{
    Map newmap;
    newmap.raiz = NULL;
    newmap.compare_llave = cmp_llave;
    newmap.copy_llave = cpy_llave;
    newmap.copy_valor = cpy_valor;
    return newmap;
}

struct _MapNode* map_crear_nodo(void* llave, void* valor, FuncionCopiadoraLlave cpy_llave, FuncionCopiadoraValor cpy_valor)
{
    struct _MapNode* newnodo = (struct _MapNode*)malloc(sizeof(struct _MapNode));
    newnodo->llave = NULL;
    newnodo->valor = NULL;
    cpy_llave(&(newnodo->llave),llave);
    cpy_valor(&(newnodo->valor),valor);
    newnodo->izq = NULL;
    newnodo->der = NULL;
    return newnodo;
}

struct _MapNode* map_insert_aux(struct _MapNode* nodo, void* llave, void* valor, FuncionComparadoraLlave cmp_llave, FuncionCopiadoraLlave cpy_llave, FuncionCopiadoraValor cpy_valor)
{
    if(nodo == NULL)
        return map_crear_nodo(llave,valor,cpy_llave,cpy_valor);
    else
    {
        int cmp = cmp_llave(llave,nodo->llave);
        if(cmp < 0)
            nodo = map_insert_aux(nodo->izq,llave,valor,cmp_llave,cpy_llave,cpy_valor);
        else if(cmp > 0)
            nodo = map_insert_aux(nodo->der,llave,valor,cmp_llave,cpy_llave,cpy_valor);
        else
            cpy_valor(&(nodo->llave),llave);
        return nodo;
    }
}

Map map_insert(Map m,void* llave, void* valor)
{
    m.raiz = map_insert_aux(m.raiz,llave,valor,m.compare_llave,m.copy_llave,m.copy_valor);
    return m;
}