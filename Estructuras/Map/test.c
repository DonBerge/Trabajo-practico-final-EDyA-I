#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "map.c"

int cmp_int(void* a, void* b)
{
    assert(a != NULL);
    assert(b != NULL);
    int va = *((int*)a), vb=*((int*)b);
    return (va > vb) - (vb > va);
}

void copy_int(void** a, void* b)
{
    if(*a == NULL)
        *a = malloc(sizeof(int));
    *((int*)*a) = *((int*)b);
}

int main()
{
    int llaves[] = {1,2,3,4,5,6};
    int valor[] = {3,5,1,2,5,7};
    Map m = map_crear(cmp_int,copy_int,copy_int);
    for(int i=0;i<6;i++)
        m = map_insert(m,llaves+i,valor+i);
}