# Compilador y opciones
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -pedantic -g
LDLIBS = -lm

OBJS = main.o simulador.o validaciones.o analisis.o

artemis: $(OBJS)
	$(CC) $(CFLAGS) -o artemis $(OBJS) $(LDLIBS)

main.o: main.c simulador.h validaciones.h analisis.h
simulador.o: simulador.c simulador.h
validaciones.o: validaciones.c validaciones.h
analisis.o: analisis.c analisis.h simulador.h

clean:
	rm -f $(OBJS) artemis

.PHONY: clean
