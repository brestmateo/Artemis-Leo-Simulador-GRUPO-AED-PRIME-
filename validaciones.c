/*
 * validaciones.c
 * Implementación del ingreso seguro de datos.
 *
 * Idea central: NO usar scanf directamente (si el usuario escribe "abc"
 * donde va un número, scanf falla y deja basura en el buffer).
 * En su lugar: leer la línea completa con fgets y convertirla nosotros
 * con strtol / strtod, comprobando si la conversión fue exitosa.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <math.h>
#include "validaciones.h"

#define TAM_LINEA 128

/* ---------- Mensajes ---------- */

void mostrar_error(const char *mensaje)
{
    printf("  [ERROR] %s\n", mensaje);
}

void mostrar_advertencia(const char *mensaje)
{
    printf("  [AVISO] %s\n", mensaje);
}

/* ---------- Funciones auxiliares (privadas: 'static') ---------- */

/* Lee una línea de stdin en 'buf'.
 * Devuelve 1 si la línea entró completa, 0 si era demasiado larga
 * (en ese caso descarta lo que sobraba para no contaminar la próxima lectura).
 * Si la entrada se cierra (Ctrl+D / fin de archivo) termina el programa
 * de forma ordenada. */
static int leer_linea(char *buf, size_t tam)
{
    size_t len;
    int c;

    if (fgets(buf, (int)tam, stdin) == NULL) {
        printf("\nEntrada finalizada. Cerrando el programa.\n");
        exit(EXIT_SUCCESS);
    }

    len = strlen(buf);
    if (len > 0 && buf[len - 1] == '\n') {
        buf[len - 1] = '\0';
        return 1;
    }

    /* No hay '\n': o es la última línea sin salto, o se pasó del tamaño. */
    c = getchar();
    if (c == EOF) {
        return 1;
    }
    while (c != '\n' && c != EOF) {
        c = getchar();
    }
    return 0;
}

/* Devuelve 1 si lo que queda del texto son solo espacios. */
static int solo_espacios(const char *s)
{
    while (*s != '\0') {
        if (!isspace((unsigned char)*s)) {
            return 0;
        }
        s++;
    }
    return 1;
}

/* ---------- Lectura de enteros ---------- */

int leer_entero(const char *mensaje, int min, int max)
{
    char buf[TAM_LINEA];
    char texto[TAM_LINEA];
    char *fin;
    long valor;

    for (;;) {
        printf("%s", mensaje);

        if (!leer_linea(buf, sizeof buf)) {
            mostrar_error("La entrada es demasiado larga.");
            continue;
        }

        errno = 0;
        valor = strtol(buf, &fin, 10);

        if (fin == buf) {
            mostrar_error("Debe ingresar un número entero.");
        } else if (!solo_espacios(fin)) {
            mostrar_error("Hay caracteres no válidos después del número.");
        } else if (errno == ERANGE || valor < min || valor > max) {
            snprintf(texto, sizeof texto,
                     "El valor debe estar entre %d y %d.", min, max);
            mostrar_error(texto);
        } else {
            return (int)valor;
        }
    }
}

/* ---------- Lectura de reales ---------- */

double leer_flotante(const char *mensaje, double min, double max)
{
    char buf[TAM_LINEA];
    char texto[TAM_LINEA];
    char *fin;
    char *coma;
    double valor;

    for (;;) {
        printf("%s", mensaje);

        if (!leer_linea(buf, sizeof buf)) {
            mostrar_error("La entrada es demasiado larga.");
            continue;
        }

        /* En Argentina se escribe "0,5": lo pasamos a "0.5". */
        coma = strchr(buf, ',');
        if (coma != NULL) {
            *coma = '.';
        }

        errno = 0;
        valor = strtod(buf, &fin);

        if (fin == buf) {
            mostrar_error("Debe ingresar un número (ej: 192.5).");
        } else if (!solo_espacios(fin)) {
            mostrar_error("Hay caracteres no válidos después del número.");
        } else if (errno == ERANGE || !isfinite(valor)) {
            mostrar_error("El número ingresado no es válido.");
        } else if (valor < min || valor > max) {
            snprintf(texto, sizeof texto,
                     "El valor debe estar entre %g y %g.", min, max);
            mostrar_error(texto);
        } else {
            return valor;
        }
    }
}

/* ---------- Lectura de caracteres ---------- */

char leer_caracter(const char *mensaje, const char *validos)
{
    char buf[TAM_LINEA];
    char texto[TAM_LINEA];
    const char *p;
    char c;

    for (;;) {
        printf("%s", mensaje);

        if (!leer_linea(buf, sizeof buf)) {
            mostrar_error("La entrada es demasiado larga.");
            continue;
        }

        /* Saltar espacios iniciales. */
        p = buf;
        while (isspace((unsigned char)*p)) {
            p++;
        }

        if (*p == '\0') {
            mostrar_error("No ingresó ningún carácter.");
            continue;
        }

        c = (char)toupper((unsigned char)*p);

        if (!solo_espacios(p + 1)) {
            mostrar_error("Ingrese un único carácter.");
        } else if (strchr(validos, c) == NULL) {
            snprintf(texto, sizeof texto,
                     "Opción inválida. Opciones posibles: %s", validos);
            mostrar_error(texto);
        } else {
            return c;
        }
    }
}
