#include "listaintervalos.c"
#include <stdio.h>

int main()
{
    int izq[] = {1,2,3,4,5,6};
    int der[] = {2,3,4,1,4,5};
    int n = sizeof(izq)/sizeof(int);
    ListaIntervalos l = lista_intervalos_crear();
    for(int i=0;i<n;i++)
        l = lista_intervalos_insertar_final(l,izq[i],der[i]);
    l = lista_intervalos_eliminar_inicio(l);
    l = lista_intervalos_eliminar_final(l);
    NodoListaIntervalos* p = l.inicio;
    while(p!=NULL)
    {
        printf("(%d %d) ",p->dato.inicio,p->dato.final);
        p = p->next;
    }
    lista_intervalos_destruir(l);
    return 0;
}