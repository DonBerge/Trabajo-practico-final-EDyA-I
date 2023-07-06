#ifndef __AUTOMATA_C__
#define __AUTOMATA_C__
#include "automata.h"
// #include "../ListaIntervalos/listaintervalos.c"
#include "../HeapIntervalos/heapintervalos.c"
#include <assert.h>
#include <ctype.h>
#include "../Cola/cola.c"
#include <stdlib.h>

typedef enum
{
    FALSO,
    VERDADERO
} Bool;

typedef struct _EstadoAutomata
{
    // Esta variable indica si la palabra a sido aceptada o no
    Bool palabraAceptada;
    // El largo del prefijo representado por el estado actual
    unsigned largoPrefijo;
    struct _EstadoAutomata *transicion[26];
    // Puntero al estado conectado al estado actual por una transicion de falla
    struct _EstadoAutomata *transicion_de_falla;

    struct _EstadoAutomata *transicion_de_salida;

    // Puntero al estado padre del nodo, un estado P es padre de otro estado N
    // si hay una transicion de P a N que no es una transicion de falla
    struct _EstadoAutomata *padre;
    // El valor de la transicion con la que estan conectados el estado actual
    // y su padre
    int valor_transicion_padre;
} EstadoAutomata;

EstadoAutomata *estado_crear(EstadoAutomata *padre, char valor_transicion)
{
    EstadoAutomata *estado = (EstadoAutomata *)malloc(sizeof(EstadoAutomata));
    assert(estado != NULL);
    estado->palabraAceptada = FALSO;
    estado->largoPrefijo = (padre == NULL) ? 0 : padre->largoPrefijo + 1;
    // Inicialmente, el nodo no tiene transicion de falla ni de salida
    estado->transicion_de_falla = NULL;
    estado->transicion_de_salida = NULL;
    estado->padre = padre;
    estado->valor_transicion_padre = ((int)valor_transicion);
    // Inicializo los hijos
    for (int i = 0; i < 26; i++)
        estado->transicion[i] = NULL;

    return estado;
}

Bool estado_transicion_disponible(EstadoAutomata *estado, char valor_transicion)
{
    int a = (int)valor_transicion - 'a';
    EstadoAutomata *b = estado->transicion[a];
    return estado != NULL && (estado->transicion[a]) != NULL;
}

Automata automata_crear(FILE *diccionario)
{
    EstadoAutomata *estado_inicial = estado_crear(NULL, '\0');
    EstadoAutomata *estado_actual = estado_inicial;
    assert(estado_actual != NULL);
    while (!feof(diccionario))
    {
        char c = tolower(fgetc(diccionario));
        // Ya se leyo una palabra
        if (c == '\n' || c == EOF)
        {
            estado_actual->palabraAceptada = VERDADERO;
            estado_actual = estado_inicial;
        }
        else
        {
            if (!estado_transicion_disponible(estado_actual, c))
                estado_actual->transicion[(int)c - 'a'] = estado_crear(estado_actual, c);
            estado_actual = estado_actual->transicion[(int)c - 'a'];
        }
    }
    estado_inicial = automata_calcular_transiciones_de_falla(estado_inicial);
    return estado_inicial;
}

void automata_destruir(Automata automata)
{
    if (automata != NULL)
    {
        for (int i = 0; i < 26; i++)
            automata_destruir(automata->transicion[i]);
        free(automata);
    }
}

void *copy_id(void *dato)
{
    return dato;
}

void void_destroy(void *dato)
{
    dato = dato;
}

// Toma un estado del automata y un caracter y sigue la trasicion correspondiente si es que existe,
// caso contrario recorre las transiciones de falla hasta encontrar una transicion valida o llegar a
// la raiz
Automata automata_seguir_transicion(Automata estado, char valor_transicion)
{
    assert(estado != NULL);
    while (estado->padre != NULL && !estado_transicion_disponible(estado, valor_transicion))
        estado = estado->transicion_de_falla;
    if (estado_transicion_disponible(estado, valor_transicion))
        estado = estado->transicion[(int)valor_transicion - 'a'];
    return estado;
}

// Dado un automata calcula todas las transiciones de falla de sus estados
Automata automata_calcular_transiciones_de_falla(Automata estado_inicial)
{
    // Recorro el automata por niveles usando BFS y una cola
    Cola c = cola_crear(copy_id, void_destroy);
    c = cola_push(c, estado_inicial);
    while (!cola_empty(c))
    {
        Automata estado_actual = (Automata)cola_front(c);
        c = cola_pop(c);

        // Si el estado actual es el inicial, tiene como padre al inicial o es un estado de aceptacion, su transicion de falla
        // apunta al estado inicial
        if (estado_actual == estado_inicial || estado_actual->padre == estado_inicial || estado_actual->palabraAceptada)
            estado_actual->transicion_de_falla = estado_inicial;
        else
            // Caso contrario sea c el caracter de la transicion que une al estado con su padre, sigo la transicion de falla del
            // padre del nodo y desde ahi sigo la transicion correspondiente a c
            estado_actual->transicion_de_falla = automata_seguir_transicion(estado_actual->padre->transicion_de_falla, estado_actual->valor_transicion_padre);

        // Calculo la transicion de salida
        if (estado_actual->transicion_de_falla->palabraAceptada)
            estado_actual->transicion_de_salida = estado_actual->transicion_de_falla;
        else
            estado_actual->transicion_de_salida = estado_actual->transicion_de_falla->transicion_de_salida;

        // Meto los estados que estan conectados por una transicion que no es de falla en la cola
        for (int i = 'a'; i <= 'z'; i++)
            if (estado_transicion_disponible(estado_actual, i))
                c = cola_push(c, estado_actual->transicion[i - 'a']);
    }
    return estado_inicial;
}

void *copy_char(void *dato)
{
    char *c = (char *)malloc(sizeof(char));
    *c = *((char *)dato);
    return c;
}

void destruir_char(void *dato)
{
    free(dato);
}

void procesarCaracteres(Cola *cola, HeapIntervalos *heap_intervalos /*ListaIntervalos *lista_intervalos*/, FILE *archivo_salida, int letrasADescartar)
{
    int ultimoFin = -1;
    for (int i = 0; i < letrasADescartar; i++)
    {
        if (heap_intervalos_vacio(*heap_intervalos))
            *cola = cola_pop(*cola);
        else
        {
            Intervalo inter = heap_intervalos_obtener_primero(*heap_intervalos);
            /*printf("%d\n", ultimoFin);
            for (int j = 0; j < heap_intervalos->ultimo; j++)
                printf("(%d %d) ", heap_intervalos->array[j].inicio, heap_intervalos->array[j].final);
            printf("\n\n");*/
            int seSaltaronLetras = 0;
            int seProcesaronLetras = 0;
            while (i < inter.inicio && i < letrasADescartar)
            {
                *cola = cola_pop(*cola);
                i++;
                seSaltaronLetras = 1;
            }
            while (i < inter.final && i < letrasADescartar)
            {
                seProcesaronLetras = 1;
                fputc(*((char *)cola_front(*cola)), archivo_salida);
                *cola = cola_pop(*cola);
                i++;
            }
            if (seProcesaronLetras)
            {
                fputc(' ', archivo_salida);
                ultimoFin = inter.final;
                *heap_intervalos = heap_intervalos_eliminar_primero(*heap_intervalos);
            }
            if (seSaltaronLetras || seProcesaronLetras)
                i--;
        }
        while (!heap_intervalos_vacio(*heap_intervalos) && ultimoFin >= heap_intervalos_obtener_primero(*heap_intervalos).inicio)
            *heap_intervalos = heap_intervalos_eliminar_primero(*heap_intervalos);
    }
}

void automata_procesar_archivo(Automata estado_inicial, FILE *archivo_entrada, FILE *archivo_salida)
{
    // Uso una cola para guardar temporalmente los caracteres que pueden formar una palabra a espaciar
    Cola cola = cola_crear(copy_char, destruir_char);

    HeapIntervalos heap_intervalos = heap_intervalos_crear();

    // Inicialmente, el estado actual es el inicial y el largo de la ultima
    // palabra encontrada es 0
    Automata estado_actual = estado_inicial;
    int largo_ultima_palabra_encontrada = 0;

    while (!feof(archivo_entrada))
    {
        char c = fgetc(archivo_entrada);

        // Si llegue al fin de una linea o al fin del archivo, hay que consumir
        // los caracteres sobrantes, reinciar el automata a como estaba inicialmente
        if (c == '\n' || c == EOF)
        {
            procesarCaracteres(&cola, &heap_intervalos, archivo_salida, cola_size(cola));
            // Dejo el automata a como estaba inicialmente
            estado_actual = estado_inicial;
            largo_ultima_palabra_encontrada = 0;

            fputc('\n', archivo_salida);
        }
        // Si no llegue al fin de linea o al fin del archivo entonces proceso
        // el caracter
        else
        {
            // Meto el caracter en la cola
            cola = cola_push(cola, &c);

            // Sigo la transicion correspondiente al caracter leido y actualizo
            // el largo de la ultima palabra encontrada si es necesario
            int prevLargo = estado_actual->largoPrefijo;
            estado_actual = automata_seguir_transicion(estado_actual, tolower(c));
            int nextLargo = estado_actual->largoPrefijo;

            // Si hay letras a descartar es necesario recuperarse de errores
            int letrasADescartar = prevLargo - estado_actual->largoPrefijo + 1;
            procesarCaracteres(&cola, &heap_intervalos, archivo_salida, letrasADescartar);

            // Se llego a un estado de aceptacion, la palabra aceptada necesariamente contiene todas las palabras vistas
            // hasta el momento, asi que se descartan las palabras encontradas hasta el momento
            if (estado_actual->palabraAceptada)
                heap_intervalos = heap_intervalos_insertar(heap_intervalos, intervalo_crear(0, estado_actual->largoPrefijo));
            // Existe un sufijo propio del prefijo actual que esta en el diccionario
            for (EstadoAutomata *estado_salida = estado_actual->transicion_de_salida; estado_salida != NULL; estado_salida = estado_salida->transicion_de_salida)
                heap_intervalos = heap_intervalos_insertar(heap_intervalos, intervalo_crear(estado_actual->largoPrefijo - estado_salida->largoPrefijo, estado_actual->largoPrefijo));
        }
    }
    heap_intervalos_destruir(heap_intervalos);
    cola_destroy(cola);
}

#endif