typedef struct _DList DList;

// Toma dos direcciones, mete el valor de la segunda direccion en la primera direccion
typedef void* (*FuncionCopia)(void*);
typedef void (*FuncionDestructora)(void*);

DList dlist_crear(FuncionCopia, FuncionDestructora);
void dlist_destruir(DList);
int dlist_vacia(DList);
DList dlist_insertar_inicio(DList,void*);
DList dlist_insertar_final(DList, void*);
DList dlist_eliminar_inicio(DList);
DList dlist_eliminar_final(DList);
void* dlist_elemento_inicio(DList);
void* dlist_elemento_final(DList);