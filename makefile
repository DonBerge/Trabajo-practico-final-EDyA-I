CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g

AUTOMATA_DIR = ./Estructuras/Automata
AUTOMATA_H = $(AUTOMATA_DIR)/automata.h
AUTOMATA_C = $(AUTOMATA_DIR)/automata.c
AUTOMATA_TEST = $(AUTOMATA_DIR)/Test/test_automata.c

COLA_DIR = ./Estructuras/Cola
HEAP_INTERVALOS_DIR = ./Estructuras/HeapIntervalos

all: test_automata main

main: cola.o heapintervalos.o automata.o main.o
	$(CC) $(CFLAGS) cola.o heapintervalos.o automata.o main.o -o main

main.o: $(AUTOMATA_DIR)/automata.h main.c
	$(CC) $(CFLAGS) -c main.c

automata.o: $(AUTOMATA_DIR)/automata.h $(AUTOMATA_DIR)/automata.c $(COLA_DIR)/cola.h $(HEAP_INTERVALOS_DIR)/heapintervalos.h
	$(CC) $(CFLAGS) -c $(AUTOMATA_DIR)/automata.c

cola.o: $(COLA_DIR)/cola.h $(COLA_DIR)/cola.c
	$(CC) $(CFLAGS) -c $(COLA_DIR)/cola.c

heapintervalos.o: $(HEAP_INTERVALOS_DIR)/heapintervalos.h $(HEAP_INTERVALOS_DIR)/heapintervalos.c
	$(CC) $(CFLAGS) -c $(HEAP_INTERVALOS_DIR)/heapintervalos.c

test_automata.o: $(AUTOMATA_DIR)/automata.h $(AUTOMATA_DIR)/automata.c $(AUTOMATA_DIR)/Test/test_automata.c
	$(CC) $(CFLAGS) -c $(AUTOMATA_DIR)/test_automata.c