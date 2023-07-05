typedef struct {
    int inicio;
    int final;
} Intervalo;

typedef struct _ListaIntervalos ListaIntervalos;

// Los intervalos en la lista se insertan de manera tal que ningun intervalo se solapa con otro
// Los intervalos deben insertarse en la lista en orden, mas especificamente en el orden de la ultima coordenada

ListaIntervalos lista_intervalos_crear();
ListaIntervalos lista_intervalos_destruir(ListaIntervalos);
int lista_intervalos_vacia(ListaIntervalos);
ListaIntervalos lista_intervalos_insertar_inicio(ListaIntervalos,int,int);
ListaIntervalos lista_intervalos_insertar_final(ListaIntervalos,int,int);
ListaIntervalos lista_intervalos_eliminar_inicio(ListaIntervalos);
ListaIntervalos lista_intervalos_eliminar_final(ListaIntervalos);
Intervalo lista_intervalos_elemento_inicio(ListaIntervalos);
Intervalo lista_intervalos_elemento_final(ListaIntervalos);