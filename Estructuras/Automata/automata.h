#ifndef __AUTOMATA_H__
#define __AUTOMATA_H__
#include <stdio.h>
// Represento al automata como un puntero al estado inicial
typedef struct _EstadoAutomata* Automata;

// Crea un automata a partir de un archivo de diccionario
Automata automata_crear(FILE*);
// Destruye un automata pasado por argumento
void automata_destruir(Automata);
// Calcula las transiciones de falla y salida del automata
Automata automata_calcular_transiciones_adicionales(Automata);
// Toma un archivo de entrada con las palabras a procesar y un
// archivo de salida donde se guardan las palabras espaciadas
void automata_procesar_archivo(Automata,FILE*,FILE*);
#endif