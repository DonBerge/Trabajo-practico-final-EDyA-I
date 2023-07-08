#include "cola.h"
#include <stdlib.h>

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
    return 0;
}