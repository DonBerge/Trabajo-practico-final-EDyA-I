#include <stdio.h>
#include "Estructuras/Automata/automata.h"

int main(int argc, char **argv)
{
    if (argc == 1)
        printf("No se indico un nombre para el archivo de diccionario");
    else if (argc == 2)
        printf("No se indico un nombre para el archivo de entrada");
    else if (argc == 3)
        printf("No se indico un nombre para el archivo de salida");
    else
    {
        FILE *diccionario = fopen(argv[1], "r");
        if (diccionario == NULL)
        {
            fprintf(stderr,"No se encontro el archivo de diccionario %s\n", argv[1]);
            return 1;
        }
        else
        {
            Automata a = automata_crear(diccionario);
            fclose(diccionario);
            FILE *entrada = fopen(argv[2], "r"), *salida = fopen(argv[3], "w");
            if (entrada == NULL)
            {
                fprintf(stderr,"No se encontro el archivo de entrada %s\n", argv[2]);
                return 1;
            }
            else if (salida == NULL)
            {
                fprintf(stderr,"Error al crear el archivo de salida %s\n", argv[3]);
                return 1;
            }
            else
            {
                automata_procesar_archivo(a, entrada, salida);
                fclose(entrada);
                fclose(salida);
                automata_destruir(a);
            }
        }
    }
    return 0;
}