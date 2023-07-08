#include "../automata.c"
#include <stdlib.h>
#include <assert.h>

#define CANT_TEST 6

void printChar(char c)
{
    if (c == '\r')
        fprintf(stderr, "\\r");
    else if (c == '\n')
        fprintf(stderr, "\\n");
    else if (c == EOF)
        fprintf(stderr, "EOF");
    else if (c == ' ')
        fprintf(stderr, "(espacio)");
    else if (c == '\t')
        fprintf(stderr, "\\t");
    else
        fprintf(stderr, "%c", c);
}

int archivos_son_iguales(FILE *arch1, FILE *arch2)
{
    int son_iguales = 1;
    int numChar = 1;
    while (son_iguales && !feof(arch1) && !feof(arch2))
    {
        char c1, c2;
        do
            c1 = fgetc(arch1);
        while (c1 == '\r');
        do
            c2 = fgetc(arch2);
        while (c2 == '\r');

        son_iguales = c1 == c2;
        if (!son_iguales)
        {
            fprintf(stderr, "Los archivos son distintos en el %d-esimo caracter(", numChar);
            printChar(c1);
            fprintf(stderr, " y ");
            printChar(c2);
            fprintf(stderr, ")\n");
        }
        numChar++;
    }
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
    char *str = (char *)malloc(20 + piso_log10(CANT_TEST));
    assert(str != NULL);
    int testsAceptados = 0;
    for (int i = 0; i < CANT_TEST; i++)
    {
        sprintf(str, "./Diccionarios/%d.txt", i + 1);
        FILE *diccionario = fopen(str, "r");
        assert(diccionario != NULL);
        Automata automata = automata_crear(diccionario);
        fclose(diccionario);
        sprintf(str, "./Entradas/%d.txt", i + 1);
        FILE *entrada = fopen(str, "r");
        assert(entrada != NULL);
        FILE *salida_automata = fopen("test_salida.txt", "w");
        assert(salida_automata != NULL);
        automata_procesar_archivo(automata, entrada, salida_automata);
        fclose(entrada);
        fclose(salida_automata);
        salida_automata = fopen("test_salida.txt", "r");
        assert(salida_automata != NULL);
        sprintf(str, "./Salidas/%d.txt", i + 1);
        FILE *salida_test = fopen(str, "r");
        int archivosSonIguales = archivos_son_iguales(salida_automata, salida_test);
        if (!archivosSonIguales)
            printf("ERROR EN EL TEST %d\n", i + 1);
        else
        {
            printf("PASO EL TEST %d\n",i+1);
            testsAceptados++;
        }
        fclose(salida_automata);
        fclose(salida_test);
        automata_destruir(automata);
    }
    printf("Pasaron %d de %d test\n",testsAceptados,CANT_TEST);
    free(str);
    return 0;
}