/*
 * aterrizaje.c
 * Juego: ORION - Aterrizaje lunar (por turnos, en consola).
 *
 * Compilar:  gcc -Wall -o aterrizaje aterrizaje.c -lm
 * Ejecutar:  ./aterrizaje
 *
 * Cada turno dura 1 segundo. Elegís cuánto combustible quemar (kg/s).
 * Aterrizá con velocidad vertical <= 3 m/s antes de quedarte sin combustible.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define G_LUNA        1.62   /* m/s^2 */
#define EMPUJE_POR_KG 0.15   /* m/s^2 de aceleración por cada kg/s quemado */
#define QUEMA_MAX     40.0   /* kg/s máximo */
#define V_SEGURA      3.0    /* m/s máxima para aterrizar bien */
#define DT            1.0    /* s por turno */

typedef struct {
    double altura;      /* m */
    double velocidad;   /* m/s (negativa = bajando) */
    double combustible; /* kg */
    int    tiempo;      /* s */
} Estado;

static void iniciar(Estado *e, int dificultad)
{
    e->altura = 800.0;
    e->velocidad = -30.0;
    e->tiempo = 0;
    switch (dificultad) {
    case 1:  e->combustible = 450.0; break;  /* fácil   */
    case 3:  e->combustible = 230.0; break;  /* difícil */
    default: e->combustible = 320.0; break;  /* normal  */
    }
}

/* Lee una línea y devuelve un double validado en [min, max]. */
static double leer_numero(const char *msg, double min, double max)
{
    char buf[64], *fin;
    double v;

    for (;;) {
        printf("%s", msg);
        if (!fgets(buf, sizeof buf, stdin)) exit(0);
        v = strtod(buf, &fin);
        if (fin != buf && (*fin == '\n' || *fin == '\0') && v >= min && v <= max)
            return v;
        printf("  Valor inválido. Ingresá un número entre %.0f y %.0f.\n", min, max);
    }
}

static void dibujar(const Estado *e)
{
    /* Barra vertical simple: la nave "baja" a medida que pierde altura. */
    int filas = 12;
    int pos = (int)(e->altura / 800.0 * (filas - 1));
    int i;

    if (pos < 0) pos = 0;
    if (pos > filas - 1) pos = filas - 1;

    printf("\n");
    for (i = filas - 1; i >= 0; i--) {
        if (i == pos) printf("      /^\\   <- Orion\n");
        else          printf("      |   |\n");
    }
    printf("  ==============  SUPERFICIE  ==============\n");
}

static void estado(const Estado *e)
{
    printf("\n t=%3d s | Altura: %7.1f m | Vel: %6.1f m/s | Comb: %6.1f kg\n",
           e->tiempo, e->altura, e->velocidad, e->combustible);
}

/* Devuelve 1 si hubo contacto con el suelo. */
static int avanzar(Estado *e, double quema)
{
    double a, disc, t, h0 = e->altura, v0 = e->velocidad;

    if (quema > e->combustible) quema = e->combustible;
    e->combustible -= quema;
    a = -G_LUNA + EMPUJE_POR_KG * quema;

    e->altura = h0 + v0 * DT + 0.5 * a * DT * DT;
    e->velocidad = v0 + a * DT;
    e->tiempo++;

    if (e->altura <= 0.0) {
        /* Tiempo exacto de contacto dentro del paso: h0 + v0 t + a t^2/2 = 0 */
        if (fabs(a) < 1e-9) {
            t = (v0 < 0) ? -h0 / v0 : DT;
        } else {
            disc = v0 * v0 - 2.0 * a * h0;
            if (disc < 0) disc = 0;
            t = (-v0 - sqrt(disc)) / a;
            if (t < 0 || t > DT) t = (-v0 + sqrt(disc)) / a;
            if (t < 0) t = 0;
            if (t > DT) t = DT;
        }
        e->velocidad = v0 + a * t;
        e->altura = 0.0;
        return 1;
    }
    return 0;
}

static void resultado(const Estado *e)
{
    double v = fabs(e->velocidad);
    int puntos;

    printf("\n==========================================\n");
    printf(" Velocidad de contacto: %.2f m/s\n", v);
    if (v <= V_SEGURA) {
        puntos = (int)(1000.0 - v * 100.0 + e->combustible * 2.0 - e->tiempo * 3.0);
        if (puntos < 0) puntos = 0;
        printf(" ¡ATERRIZAJE EXITOSO! Orion está en la Luna.\n");
        printf(" Combustible sobrante: %.1f kg\n", e->combustible);
        printf(" PUNTAJE: %d\n", puntos);
    } else if (v <= 8.0) {
        printf(" Aterrizaje DURO. La nave quedó dañada, pero la tripulación sobrevive.\n");
    } else {
        printf(" IMPACTO. Orion se estrelló contra la superficie.\n");
    }
    printf("==========================================\n");
}

static void jugar(int dificultad)
{
    Estado e;
    double quema;
    int tocado = 0;

    iniciar(&e, dificultad);
    printf("\nObjetivo: tocar suelo a <= %.0f m/s. Quemar 0 = caída libre.\n", V_SEGURA);
    printf("Con quema de ~%.0f kg/s la nave se mantiene flotando (compensa la gravedad).\n",
           G_LUNA / EMPUJE_POR_KG);

    while (!tocado) {
        dibujar(&e);
        estado(&e);

        if (e.combustible <= 0.0) {
            printf(" ¡SIN COMBUSTIBLE! Caída libre...\n");
            quema = 0.0;
        } else {
            quema = leer_numero("  Quema (kg/s) [0 a 40]: ", 0.0, QUEMA_MAX);
        }
        tocado = avanzar(&e, quema);
    }
    resultado(&e);
}

int main(void)
{
    int dif;
    char resp[16];

    printf("==========================================\n");
    printf("   ORION - ATERRIZAJE LUNAR\n");
    printf("==========================================\n");

    do {
        printf("\nDificultad: 1) Fácil  2) Normal  3) Difícil\n");
        dif = (int)leer_numero("Elegí: ", 1, 3);
        jugar(dif);

        printf("\n¿Jugar de nuevo? (s/n): ");
        if (!fgets(resp, sizeof resp, stdin)) break;
    } while (resp[0] == 's' || resp[0] == 'S');

    printf("\n¡Buen viaje, Orion!\n");
    return 0;
}
