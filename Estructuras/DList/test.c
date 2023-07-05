#include "dlist.c"
#include <stdio.h>

void* copy_int(void* dato)
{
    int* n = (int*)malloc(sizeof(int));
    *n = *((int*)dato);
    return (void*)n;
}

void destroy_int(void* dato)
{
    free(dato);
}

int main()
{
    int arr[] = {1,2,3,4,5,6};
    int n = sizeof(arr)/sizeof(int);
    DList l = dlist_crear(copy_int,destroy_int);
    for(int i=0;i<n;i++)
        l = dlist_insertar_final(l,arr+i);
    l = dlist_eliminar_inicio(l);
    l = dlist_eliminar_final(l);
    DNodo* p = l.inicio;
    while(p!=NULL)
    {
        printf("%d ",*((int*)p->dato));
        p = p->sig;
    }
    dlist_destruir(l);
    return 0;
}