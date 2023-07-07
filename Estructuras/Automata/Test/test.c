#include "../automata.c"
#include <stdlib.h>
#include <assert.h>

#define CANT_TEST 5

int archivos_son_iguales(FILE *arch1, FILE *arch2)
{
    int son_iguales = 1;
    while (son_iguales && !feof(arch1) && !feof(arch2))
        son_iguales = fgetc(arch1) == fgetc(arch2);
    return son_iguales;
}

int piso_log10(int n)
{
    int ans = 0;
    while (n > 0)
    {
        ans++;
        n /= 10;
    }
    return ans;
}

int main()
{
    char *str = (char *)malloc(sizeof(20 + piso_log10(CANT_TEST)));
    assert(str != NULL);
    for (int i = 0; i < CANT_TEST; i++)
    {
        sprintf(str, "./Diccionarios/%d.txt", i + 1);
        FILE *diccionario = fopen(str, "r");
        assert(diccionario != NULL);
        Automata a = automata_crear(diccionario);
        fclose(diccionario);
        sprintf(str, "./Entradas/%d.txt", i + 1);
        FILE *entrada = fopen(str, "r");
        assert(entrada != NULL);
        FILE *salida_automata = fopen("test_salida.txt", "w");
        assert(salida_automata != NULL);
        automata_procesar_archivo(a, entrada, salida_automata);
        fclose(salida_automata);
        salida_automata = fopen("test_salida.txt", "r");
        assert(salida_automata != NULL);
        sprintf(str, "./Salidas/%d.txt", i + 1);
        FILE *salida_test = fopen(str, "r");
        int archivosSonIguales = archivos_son_iguales(salida_automata,salida_test);
        if(!archivosSonIguales)
        {
            printf("ERROR TEST %d\n",i+1);
            assert(0);
        }
        fclose(salida_automata);
        fclose(salida_test);
    }
    free(str);
    return 0;
}