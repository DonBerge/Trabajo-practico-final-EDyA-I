#include "Estructuras/Automata/automata.c"
#include <stdio.h>

int main()
{
    FILE* diccionario = fopen("diccionario.txt","r");
    Automata a = automata_crear(diccionario);
    fclose(diccionario);
    FILE* entrada = fopen("entrada.txt","r"), *salida = fopen("salida.txt","w");
    automata_procesar_archivo(a,entrada,salida);
    fclose(entrada);
    fclose(salida);
    return 0;
}