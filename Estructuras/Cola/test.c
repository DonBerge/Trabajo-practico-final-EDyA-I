#include "cola.c"
#include <assert.h>
#include <stdio.h>
void cpy_int(void** a, void* b)
{
    int* ia = *a;
    if(ia == NULL)
        ia = (int*)malloc(sizeof(int));
    *ia = *((int*)b);
    *a = (void*)ia;
}

void dst_int(void* a)
{
    free(a);
}

int main()
{
    int x[] = {1,2,3,4,5,6,7,8,9,10};
    int n = sizeof(x)/sizeof(int);
    Cola c = cola_crear(cpy_int,dst_int);
    c = cola_push(c,x+5);
    c = cola_pop(c);
    c = cola_push(c,x+4);
    cola_destroy(c);
    c = cola_crear(cpy_int,dst_int);
    for(int i=0;i<n;i++)
        c = cola_push(c,x+i);
    for(int i=0;i<n/2;i++)
    {
        int* dato = (int*)cola_front(c);
        //printf("%d ",*dato);
        assert(*dato == x[i]);
        c = cola_pop(c);
    }

    for(int i=0;i<n/2;i++)
        c = cola_push(c,x+i);
    for(int i=0;i<n;i++)
    {
        int* dato = (int*)cola_front(c);
        assert(*dato == x[(i + n/2) % n]);
        c = cola_pop(c);
    }

    for(int i=0;i<n;i++)
        c = cola_push(c,x+i);
    for(int i=0;i<n;i++)
    {
        int* dato = (int*)cola_front(c);
        assert(*dato == x[i]);
        c = cola_pop(c);
    }
    cola_destroy(c);
    return 0;
}