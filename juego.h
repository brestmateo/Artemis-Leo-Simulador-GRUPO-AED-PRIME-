/*
 * juego.h
 * Misión Artemis - Situación 2: JUEGO "Misión de corrección orbital".
 *
 * Reglas (resumen):
 *   - Orion da vueltas a la Tierra en una órbita circular.
 *   - En cada turno ocurre un evento que empuja a la nave y deforma la órbita.
 *   - El jugador corrige con combustible limitado (el combustible se mide
 *     en m/s de cambio de velocidad disponibles).
 *   - Si el punto más bajo de la órbita (perigeo) queda a menos de
 *     ALT_MIN_SEGURA sobre la superficie, la nave se quema en la atmósfera.
 *   - Se gana si se sobrevive los N_TURNOS turnos.
 *
 * Simplificación (se aclara en el README): todos los empujes ocurren en el
 * mismo punto de la órbita, así la nave siempre está a la misma distancia
 * r del centro de la Tierra y solo cambian sus velocidades.
 */
#ifndef JUEGO_H
#define JUEGO_H

#include "simulador.h"

/* ---------- Constantes del juego ---------- */
#define N_TURNOS        5          /* Cantidad de turnos de la misión         */
#define N_EVENTOS       6          /* Cantidad de tipos de evento             */
#define ALT_MIN_SEGURA  150000.0   /* Perigeo mínimo para no quemarse [m]     */
#define DV_MIN_CORRECCION 1.0      /* Corrección mínima por turno [m/s]       */

/* ---------- Tipos de datos ---------- */

/* Un tipo de evento. Se guardan todos en una tabla (arreglo) en juego.c.
 * Los factores dicen hacia dónde empuja: radial (+ = hacia el espacio,
 * - = hacia la Tierra) y tangencial (+ = acelera, - = frena). */
typedef struct {
    const char *descripcion;       /* Texto que se muestra al jugador        */
    const char *nombre_corto;      /* Para la tabla del reporte final        */
    double factor_radial;
    double factor_tangencial;
    char   accion_sugerida;        /* Pista para la dificultad fácil         */
} TipoEvento;

/* Lo que pasó en un turno (se guarda en el historial de la partida). */
typedef struct {
    int    turno;
    int    evento;                 /* Índice en la tabla de eventos          */
    double magnitud;               /* Fuerza del evento [m/s]                */
    char   accion;                 /* A, F, S, B o N                         */
    double dv_gastado;             /* Combustible usado en el turno [m/s]    */
    double perigeo_km;             /* Altitud del perigeo al terminar [km]   */
    double excentricidad;          /* 0 = órbita circular                    */
} Maniobra;

/* Todo el estado de una partida. */
typedef struct {
    double radio;                  /* Distancia al centro de la Tierra [m]   */
    double v_radial;               /* Velocidad hacia afuera (+) [m/s]       */
    double v_tang;                 /* Velocidad de giro alrededor [m/s]      */
    double combustible;            /* Cambio de velocidad disponible [m/s]   */
    double combustible_inicial;    /* Para calcular el rango final           */
    int    dificultad;             /* 1 fácil, 2 normal, 3 difícil           */
    int    turnos_jugados;
    Maniobra historial[N_TURNOS];  /* Un registro por turno                  */
} Partida;

/* Tabla de eventos (definida en juego.c). */
extern const TipoEvento EVENTOS[N_EVENTOS];

/* ---------- Prototipos ---------- */

/* Prepara una partida nueva con la nave en órbita circular. */
void iniciar_partida(Partida *p, const Nave *nave, int dificultad);

/* Con la velocidad actual calcula cómo es la órbita: altitudes del perigeo
 * y del apogeo [m] y excentricidad. Devuelve 1 si la nave sigue ligada a la
 * Tierra, 0 si ya escapó (en ese caso no completa los resultados). */
int calcular_orbita_estado(double radio, double v_radial, double v_tang,
                           double *alt_perigeo, double *alt_apogeo,
                           double *excentricidad);

/* Sortea la fuerza de un evento según la dificultad [m/s]. */
double sortear_magnitud(int dificultad);

/* Aplica el evento 'id' con la fuerza 'magnitud' a la partida. */
void aplicar_evento(Partida *p, int id, double magnitud);

/* Aplica la corrección del jugador ('A','F','S','B' o 'N') gastando 'dv'
 * de combustible. Devuelve 1 si se pudo, 0 si la acción es inválida o
 * no alcanza el combustible. */
int aplicar_correccion(Partida *p, char accion, double dv);

/* Juega una misión completa (menú del juego, turnos y reporte final). */
void jugar_mision(const Nave *nave);

#endif /* JUEGO_H */
