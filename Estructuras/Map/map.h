
typedef struct _Map Map;

typedef void (*FuncionCopiadoraLlave)(void**,void*);
// Toma el dato del segundo puntero y lo mete en el primero
typedef void (*FuncionCopiadoraValor)(void**,void*);
typedef int (*FuncionComparadoraLlave)(void*,void*);

Map map_crear(FuncionComparadoraLlave,FuncionCopiadoraLlave,FuncionCopiadoraValor);
void map_destruir();
Map map_insert(Map m,void* llave, void* valor);
void* map_acceder(Map,void* llave);