CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g

AUTOMATA_DIR = ./Estructuras/Automata
AUTOMATA_H = $(AUTOMATA_DIR)/automata.h
AUTOMATA_C = $(AUTOMATA_DIR)/automata.c
AUTOMATA_TEST = $(AUTOMATA_DIR)/Test/test_automata.c

COLA_DIR = ./Estructuras/Cola
BOLSA_INTERVALOS_DIR = ./Estructuras/BolsaIntervalos/Heap

OBJDIR = obj

all: $(AUTOMATA_DIR)/Test/test_automata main

main: $(OBJDIR)/cola.o $(OBJDIR)/automata.o $(OBJDIR)/bolsaintervalos.o $(OBJDIR)/main.o
	$(CC) $(CFLAGS) $(OBJDIR)/cola.o $(OBJDIR)/bolsaintervalos.o $(OBJDIR)/automata.o $(OBJDIR)/main.o -o main

$(AUTOMATA_DIR)/Test/test_automata: $(OBJDIR)/bolsaintervalos.o  $(AUTOMATA_TEST) $(OBJDIR)/automata.o $(OBJDIR)/cola.o
	$(CC) $(CFLAGS) $(AUTOMATA_TEST) $(OBJDIR)/bolsaintervalos.o $(OBJDIR)/automata.o $(OBJDIR)/cola.o -o $(AUTOMATA_DIR)/Test/test_automata

$(OBJDIR)/main.o: $(AUTOMATA_H) main.c
	mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -o $(OBJDIR)/main.o -c main.c

$(OBJDIR)/automata.o: $(AUTOMATA_H) $(AUTOMATA_C) $(COLA_DIR)/cola.h $(BOLSA_INTERVALOS_DIR)/bolsaintervalos.h
	mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -o $(OBJDIR)/automata.o -c $(AUTOMATA_DIR)/automata.c

$(OBJDIR)/cola.o: $(COLA_DIR)/cola.h $(COLA_DIR)/cola.c
	mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -o $(OBJDIR)/cola.o -c $(COLA_DIR)/cola.c

$(OBJDIR)/bolsaintervalos.o: $(BOLSA_INTERVALOS_DIR)/bolsaintervalos.h $(BOLSA_INTERVALOS_DIR)/bolsaintervalos.c
	mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -o $(OBJDIR)/bolsaintervalos.o -c $(BOLSA_INTERVALOS_DIR)/bolsaintervalos.c