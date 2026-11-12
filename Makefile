# Compilador y opciones
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -pedantic -g
LDLIBS = -lm

OBJS = main.o simulador.o validaciones.o analisis.o juego.o

artemis: $(OBJS)
	$(CC) $(CFLAGS) -o artemis $(OBJS) $(LDLIBS)

main.o: main.c simulador.h validaciones.h analisis.h juego.h
juego.o: juego.c juego.h simulador.h validaciones.h
simulador.o: simulador.c simulador.h
validaciones.o: validaciones.c validaciones.h
analisis.o: analisis.c analisis.h simulador.h

clean:
	rm -f $(OBJS) artemis

.PHONY: clean
