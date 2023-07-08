#ifndef __COLA__H__
#define __COLA__H__

// Toma dos direcciones, mete el valor de la segunda direccion en la primera direccion, si la primera
// direccion apunta a NULL, se reserva memoria para que pueda guardar el dato
typedef void (*FuncionCopia)(void**, void*);
// Libera la memoria dinamica reservada en la direccion pasada como argumento, si es necesario
typedef void (*FuncionDestructora)(void*);

typedef struct _ColaNode
{
    void *dato;
    struct _ColaNode *sig;
} ColaNode;

typedef struct
{
    ColaNode* front;
    ColaNode* back;
    FuncionCopia copy;
    FuncionDestructora destroy;
    int size;
} Cola;

Cola cola_crear(FuncionCopia,FuncionDestructora);
int cola_empty();
int cola_size();
Cola cola_push(Cola,void*);
Cola cola_pop(Cola);
void* cola_front(Cola);
void* cola_back(Cola);
void cola_destroy(Cola c);
#endif