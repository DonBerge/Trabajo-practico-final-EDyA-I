// La bolsa de intervalos es una estructura de datos que almacena intervalos enteros
// tiene que permitir en un momento dado extraer aquel intervalo que tiene mayor longitud
// y que tiene menor coordenada inicial

typedef struct {
    int inicio;
    int final;
} Intervalo;

Intervalo intervalo_crear(int,int);

typedef struct _BolsaIntervalos* BolsaIntervalos;

BolsaIntervalos bolsa_intervalos_crear(int capacidad);
void bolsa_intervalos_destruir(BolsaIntervalos);
int bolsa_intervalos_vacia(BolsaIntervalos);
BolsaIntervalos bolsa_intervalos_insertar(BolsaIntervalos, Intervalo);
Intervalo bolsa_intervalos_obtener_primero(BolsaIntervalos);
BolsaIntervalos bolsa_intervalos_eliminar_primero(BolsaIntervalos);