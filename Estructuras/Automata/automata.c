#ifndef __AUTOMATA_C__
#define __AUTOMATA_C__

#include <assert.h>
#include <ctype.h>
#include <stdlib.h>

#include "automata.h"
#include "../Cola/cola.h"
#include "../BolsaIntervalos/Heap/bolsaintervalos.h"

typedef struct _EstadoAutomata
{
    // Esta variable indica si la palabra a sido aceptada o no
    int palabraAceptada;
    // El largo del prefijo representado por el estado actual, tambien puede entenderse
    // como su profundidad-1, al alinear el automata como un arbol
    unsigned largoPrefijo;
    struct _EstadoAutomata *transicion[26];
    // Puntero al estado conectado al estado actual por una transicion de falla
    // Las transiciones de falla apuntan al sufijo propio mas grande del prefijo actual
    // que a su vez no contiene un prefijo que es sufijo propio de una palabra del diccionario
    struct _EstadoAutomata *transicionDeFalla;

    // Puntero al estado conectao al estado actual por una transicion de salida
    // La transicion de salida apunta al sufijo propio mas grande del prefijo actual
    // que es a su vez una palabra del diccionario
    struct _EstadoAutomata *transicionDeSalida;

    // Puntero al estado padre del nodo, un estado P es padre de otro estado N
    // si hay una transicion de P a N que no es una transicion de falla
    struct _EstadoAutomata *padre;
    // El valor de la transicion con la que estan conectados el estado actual
    // y su padre
    int valorTransicionPadre;
} EstadoAutomata;

EstadoAutomata *estado_crear(EstadoAutomata *padre, char valor_transicion)
{
    EstadoAutomata *estado = (EstadoAutomata *)malloc(sizeof(EstadoAutomata));
    assert(estado != NULL);
    estado->palabraAceptada = 0;
    // La raiz tiene como prefijo la cadena vacia, su largo prefijo es 0
    estado->largoPrefijo = (padre == NULL) ? 0 : padre->largoPrefijo + 1;
    // Inicialmente, el nodo no tiene transicion de falla ni de salida
    estado->transicionDeFalla = NULL;
    estado->transicionDeSalida = NULL;
    estado->padre = padre;
    estado->valorTransicionPadre = ((int)valor_transicion);
    // Inicializo los hijos
    for (int i = 0; i < 26; i++)
        estado->transicion[i] = NULL;

    return estado;
}

int estado_transicion_disponible(EstadoAutomata *estado, char valor_transicion)
{
    return estado != NULL && (estado->transicion[(int)valor_transicion - 'a']) != NULL;
}

Automata automata_crear(FILE *diccionario)
{
    EstadoAutomata *estadoInicial = estado_crear(NULL, '\0');
    EstadoAutomata *estadoActual = estadoInicial;
    assert(estadoActual != NULL);
    while (!feof(diccionario))
    {
        // Se lee un caracter del diccionario
        char c = tolower(fgetc(diccionario));

        // Solo se permiten caracteres de fin de linea/archivo o letras del abecedario
        if (!(c == '\r' || c == '\n' || c == EOF || ('a' <= c && c <= 'z')))
        {
            fprintf(stderr, "CARACTER INVALIDO %c(%d)\n", c, c);
            assert(0);
        }

        if (c != '\r')
        {
            // Ya se leyo una palabra
            if (c == '\n' || c == EOF)
            {
                // La palabra vacia no puede ser aceptada por el automata, asi que si se ingreso
                // una linea vacia esta se saltea, de lo contrario el estado actual es un estado
                // de aceptacion
                if (estadoActual != estadoInicial)
                {
                    estadoActual->palabraAceptada = 1;
                    // Pongo el estado actual a la raiz para procesar otra palabra
                    estadoActual = estadoInicial;
                }
            }
            else
            {
                // Si no hay una transicion por el caracter c, se crea un estado para esta transicion
                // y se conectan el estado actual y el nuevo estado por el caracter c
                if (!estado_transicion_disponible(estadoActual, c))
                    estadoActual->transicion[(int)c - 'a'] = estado_crear(estadoActual, c);
                estadoActual = estadoActual->transicion[(int)c - 'a'];
            }
        }
    }
    // Ya no se ingresan mas palabras, procedo a calcular las transiciones de falla y salida
    estadoInicial = automata_calcular_transiciones_adicionales(estadoInicial);
    return estadoInicial;
}

// Libera la memoria ocupada por el automata
void automata_destruir(Automata automata)
{
    if (automata != NULL)
    {
        for (int i = 0; i < 26; i++)
            automata_destruir(automata->transicion[i]);
        free(automata);
    }
}

/*void *copy_id(void *dato)
{
    return dato;
}*/

void copy_id(void **dato1, void *dato2)
{
    *dato1 = dato2;
}

void void_destroy(void* dato __attribute__((unused)))
{
    return;
}

// Toma un estado del automata y un caracter y sigue la trasicion correspondiente si es que existe,
// caso contrario recorre las transiciones de falla hasta encontrar una transicion valida o llegar a
// la raiz
Automata automata_seguir_transicion(Automata estado, char valor_transicion)
{
    assert(estado != NULL);
    while (estado->padre != NULL && !estado_transicion_disponible(estado, valor_transicion))
        estado = estado->transicionDeFalla;
    if (estado_transicion_disponible(estado, valor_transicion))
        estado = estado->transicion[(int)valor_transicion - 'a'];
    return estado;
}

// Dado un automata calcula todas las transiciones de falla de sus estados
Automata automata_calcular_transiciones_adicionales(Automata estadoInicial)
{
    // Recorro el automata por niveles usando BFS y una cola
    // No es necesario crear copias del estado del automata, asi que
    // la funcion de copia es la identidad y la funcion destructora no hace
    // nada
    Cola colaEstados = cola_crear(copy_id, void_destroy);

    // Meto el estado inicial en la cola
    colaEstados = cola_push(colaEstados, estadoInicial);

    while (!cola_empty(colaEstados))
    {
        Automata estadoActual = (Automata)cola_front(colaEstados);
        colaEstados = cola_pop(colaEstados);

        // Si el estado actual es el inicial, tiene como padre al inicial o es un estado de aceptacion, su transicion de falla
        // apunta al estado inicial
        if (estadoActual == estadoInicial || estadoActual->padre == estadoInicial || estadoActual->palabraAceptada)
            estadoActual->transicionDeFalla = estadoInicial;
        else
            // Caso contrario sea c el caracter de la transicion que une al estado con su padre, sigo la transicion de falla del
            // padre del nodo y desde ahi sigo la transicion correspondiente a c
            estadoActual->transicionDeFalla = automata_seguir_transicion(estadoActual->padre->transicionDeFalla, estadoActual->valorTransicionPadre);

        // Calculo la transicion de salida
        if (estadoActual->transicionDeFalla->palabraAceptada)
            estadoActual->transicionDeSalida = estadoActual->transicionDeFalla;
        else
            estadoActual->transicionDeSalida = estadoActual->transicionDeFalla->transicionDeSalida;

        // Meto los estados que estan conectados por una transicion que no es de falla en la cola
        for (int i = 'a'; i <= 'z'; i++)
            if (estado_transicion_disponible(estadoActual, i))
                colaEstados = cola_push(colaEstados, estadoActual->transicion[i - 'a']);
    }
    cola_destroy(colaEstados);
    return estadoInicial;
}

/*void *copy_char(void *dato)
{
    char *c = (char *)malloc(sizeof(char));
    *c = *((char *)dato);
    return c;
}*/

void copy_char(void **dato1, void *dato2)
{
    char *c = *dato1;
    if (c == NULL)
        c = (char *)malloc(sizeof(char));
    *c = *((char *)dato2);
    *dato1 = (void *)c;
}

void destruir_char(void* dato)
{
    free(dato);
}

void procesarCaracteres(Cola *cola, BolsaIntervalos *bolsaIntervalos, FILE *archivoSalida, int letrasADescartar, int *imprimirEspacio, int indice)
{
    // Posicion del ultimo caracter procesado
    int ultimoFin = -1;
    for (int i = 0; i < letrasADescartar && !cola_empty(*cola); i++)
    {
        // Si no hay intervalos para procesar, se descartan caracteres
        if (bolsa_intervalos_vacia(*bolsaIntervalos))
            *cola = cola_pop(*cola);
        else
        {
            // Obtengo el intervalo a procesar
            Intervalo inter = bolsa_intervalos_obtener_primero(*bolsaIntervalos);
            int seProcesaranLetras = inter.inicio <= letrasADescartar + indice;

            // Si se procesaran letras, entonces hay que imprimir un espacio si es necesario
            if (seProcesaranLetras)
            {
                if (*imprimirEspacio)
                    fputc(' ', archivoSalida);
                // Se va a imprimir una palabra, todas las palabras siguientes tienen que tener un espacio
                *imprimirEspacio = 1;
            }

            // Descarto los caracteres que no se van a procesar
            while (indice + i < inter.inicio && i < letrasADescartar && !cola_empty(*cola))
            {
                *cola = cola_pop(*cola);
                i++;
            }

            // Los caracteres que se van a procesar se imprimen
            if (seProcesaranLetras)
            {
                while (indice + i <= inter.final && !cola_empty(*cola))
                {
                    fputc(*((char *)cola_front(*cola)), archivoSalida);
                    *cola = cola_pop(*cola);
                    i++;
                }
                ultimoFin = indice + i - 1;
                i--;
            }
        }
        // Puede haber intervalos que se solapen con la ultima palabra procesada, de ser asi, tienen que ser eliminados de la cola de prioridad
        while (!bolsa_intervalos_vacia(*bolsaIntervalos) && (bolsa_intervalos_obtener_primero(*bolsaIntervalos).inicio <= indice || ultimoFin >= bolsa_intervalos_obtener_primero(*bolsaIntervalos).inicio))
        {
            *bolsaIntervalos = bolsa_intervalos_eliminar_primero(*bolsaIntervalos);
        }
    }
}

void automata_procesar_archivo(Automata estadoInicial, FILE *archivoEntrada, FILE *archivoSalida)
{
    // Uso una cola para guardar temporalmente los caracteres que pueden formar una palabra a espaciar
    Cola cola = cola_crear(copy_char, destruir_char);

    // heap_intervalos es usado como una cola de prioridad, permite obtener los intervalos que tienen menor coordenada
    // inicial y que tienen mayor largo
    BolsaIntervalos bolsaIntervalos = bolsa_intervalos_crear(10);

    // Inicialmente, el estado actual es el inicial
    Automata estadoActual = estadoInicial;

    // Si esta variable tiene valor 1, se imprimira un espacio antes de mostrar una palabra encontrada
    // Cuando se encuentra e imprime la primer palabra, su valor cambia a 1. De esta forma la segunda palabra en adelante
    // queda con un espacio al inicio y todas las palabras quedan espaciadas
    int seEncontroUnaPalabra = 0;

    // Indice de la letra actual en la palabra a analizar
    int indice = 0;

    // Empiezo a leer el archivo de entrada
    while (!feof(archivoEntrada))
    {
        char c = fgetc(archivoEntrada);

        // Solo se permiten caracteres de fin de linea/archivo o letras del abecedario
        if (!(c == '\r' || c == '\n' || c == EOF || ('a' <= tolower(c) && tolower(c) <= 'z')))
        {
            fprintf(stderr, "CARACTER INVALIDO %c(%d)\n", c, c);
            assert(0);
        }

        // Si llegue al fin de una linea o al fin del archivo, hay que consumir
        // los caracteres sobrantes y reinciar el automata a como estaba inicialmente
        if (c == '\r' || c == '\n' || c == EOF)
        {
            if (c == '\r')
                fgetc(archivoEntrada);
            procesarCaracteres(&cola, &bolsaIntervalos, archivoSalida, cola_size(cola), &seEncontroUnaPalabra, indice);
            // Imprimo un caracter de fin de linea si es necesario
            if (c != EOF)
                fputc('\n', archivoSalida);
            // Dejo el automata a como estaba inicialmente
            seEncontroUnaPalabra = 0;
            estadoActual = estadoInicial;
            indice = 0;
            assert(indice == 0);
        }
        // Si no llegue al fin de linea o al fin del archivo entonces proceso
        // el caracter
        else
        {
            // Meto el caracter en la cola y aumento el indice
            cola = cola_push(cola, &c);

            // Sigo la transicion correspondiente al caracter leido y actualizo
            // el largo de la ultima palabra encontrada si es necesario
            int prevLargo = estadoActual->largoPrefijo;
            estadoActual = automata_seguir_transicion(estadoActual, tolower(c));

            // Si hay letras a descartar es necesario recuperarse de errores
            int letrasADescartar = prevLargo - estadoActual->largoPrefijo + 1;

            // Proceso los caracteres si es necesario
            procesarCaracteres(&cola, &bolsaIntervalos, archivoSalida, letrasADescartar, &seEncontroUnaPalabra, indice);
            indice += letrasADescartar;

            // Se llego a un estado de aceptacion, meto el intervalo que representa la palabra actual a la lista de intervalos
            if (estadoActual->palabraAceptada)
                bolsaIntervalos = bolsa_intervalos_insertar(bolsaIntervalos, intervalo_crear(indice, indice + estadoActual->largoPrefijo - 1));
            // Si existe algun sufijo propio de la palabra actual que esta en el diccionario, se inserta su intervalo correspondiente en la cola
            // de prioridad
            for (EstadoAutomata *estado_salida = estadoActual->transicionDeSalida; estado_salida != NULL; estado_salida = estado_salida->transicionDeSalida)
                bolsaIntervalos = bolsa_intervalos_insertar(bolsaIntervalos, intervalo_crear(indice + estadoActual->largoPrefijo - estado_salida->largoPrefijo, indice + estadoActual->largoPrefijo - 1));
        }
    }
    bolsa_intervalos_destruir(bolsaIntervalos);
    cola_destroy(cola);
}

#endif