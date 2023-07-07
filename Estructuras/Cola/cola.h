#ifndef __COLA__H__
#define __COLA__H__
typedef struct _Cola Cola;

// Toma dos direcciones, mete el valor de la segunda direccion en la primera direccion
typedef void (*FuncionCopia)(void**, void*);
typedef void (*FuncionDestructora)(void*);
typedef void (*FuncionVisitante)(void*);

Cola cola_crear(FuncionCopia,FuncionDestructora);
int cola_empty();
int cola_size();
Cola cola_push(Cola,void*);
Cola cola_pop(Cola);
void* cola_front(Cola);
void* cola_back(Cola);
void cola_visitar(Cola,FuncionVisitante);
void cola_destroy(Cola c);
#endif