#include "bolsaintervalos.c"
#include <assert.h>

void printTabla(BolsaIntervalos b)
{
    for(unsigned i=0;i<b->capacidad; i++)
    {
        for(CasillaHash* c = b->casillas[i]; c != NULL; c = c->sig)
            printf("%d ", c->key);
        printf("\n");
    }
}

int main()
{
    int inicios[] = {2,1,1,9,1,22,2};
    int fin[] = {7,2,3,10,9,26,5};
    int n = sizeof(inicios)/sizeof(int);
    BolsaIntervalos b = bolsa_intervalos_crear(n);
    for(int i=0;i<n;i++)
        b = bolsa_intervalos_insertar(b,intervalo_crear(inicios[i],fin[i]));
    
    
    while(!bolsa_intervalos_vacia(b))
    {
        Intervalo x = bolsa_intervalos_obtener_primero(b);
        printf("(%d %d)\n",x.inicio,x.final);
        b = bolsa_intervalos_eliminar_primero(b);
    }
    
    bolsa_intervalos_destruir(b);
}