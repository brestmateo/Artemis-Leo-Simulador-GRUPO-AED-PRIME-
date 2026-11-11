/*
 * simulador.h
 * Misión Artemis - Situación 2: Inserción y estabilidad en órbita LEO (MCU).
 *
 * Es el "contrato" del módulo de lógica: acá se declaran las constantes,
 * los tipos de datos y los prototipos de las funciones públicas.
 * No se implementa nada acá (eso va en simulador.c).
 */
#ifndef SIMULADOR_H
#define SIMULADOR_H

/* ---------- Constantes físicas (SI) ---------- */
#define G_GRAV      6.674e-11      /* Constante de gravitación universal [N·m²/kg²] */
#define M_TIERRA    5.972e24       /* Masa de la Tierra [kg]                        */
#define R_TIERRA    6371000.0      /* Radio medio terrestre [m]                     */
#define PI          3.14159265358979323846

/* ---------- Valores por defecto del escenario ---------- */
#define M_ORION     33000.0        /* Masa de la nave Orion [kg]                    */
#define H_DEFECTO   192000.0       /* Altitud orbital del enunciado [m]             */
#define A_PULSO     0.50           /* Aceleración del propulsor lateral [m/s²]      */
#define DT_PULSO    10.0           /* Duración del pulso [s]                        */

/* ---------- Límites de validación para la entrada del usuario ---------- */
#define H_MIN       160000.0       /* Altitud mínima LEO [m]  (~160 km)             */
#define H_MAX       2000000.0      /* Altitud máxima LEO [m]  (2000 km)             */
#define MASA_MIN    1000.0         /* Masa mínima de la nave [kg]                   */
#define MASA_MAX    1000000.0      /* Masa máxima de la nave [kg]                   */
#define A_PULSO_MAX 10.0           /* Aceleración máxima del pulso [m/s²]           */
#define DT_PULSO_MAX 600.0         /* Duración máxima del pulso [s]                 */

/* ---------- Tipos de datos ---------- */

/* Parámetros de la órbita circular (MCU) de la nave. */
typedef struct {
    double altitud;          /* h sobre la superficie [m]                */
    double radio;            /* r = R_TIERRA + h [m]                     */
    double velocidad;        /* v orbital [m/s]                          */
    double periodo;          /* T [s]                                    */
    double acel_centripeta;  /* a_c = v²/r [m/s²]                        */
    double fuerza_grav;      /* F = G·M·m/r² [N]                         */
} Orbita;

/* La nave: su masa y la órbita en la que está. */
typedef struct {
    double masa;             /* [kg] */
    Orbita orbita;
} Nave;

/* Resultado de aplicar un pulso lateral de aceleración. */
typedef struct {
    double delta_v;          /* Δv = a·Δt [m/s]                              */
    double desviacion;       /* desplazamiento lateral ½·a·Δt² [m]           */
    double v_nueva;          /* módulo de la nueva velocidad [m/s]           */
    double excentricidad;    /* e de la órbita resultante (0 = circular)     */
    double semieje;          /* semieje mayor de la nueva órbita [m]         */
} ResultadoPulso;

/* ---------- Prototipos de funciones públicas ---------- */

/* Calcula y guarda en la nave todos los parámetros de su órbita circular. */
void inicializar_nave(Nave *nave, double altitud_m, double masa_kg);

double calcular_velocidad_orbital(double radio);
double calcular_periodo(double radio, double velocidad);
double calcular_aceleracion_centripeta(double velocidad, double radio);
double calcular_fuerza_gravitatoria(double masa, double radio);

/* Pulso perpendicular a la velocidad, de aceleración a durante dt segundos. */
ResultadoPulso calcular_pulso_lateral(const Nave *nave, double a, double dt);

/* Funciones que imprimen cada inciso del enunciado. */
void mostrar_inciso_a(const Nave *nave);
void mostrar_inciso_b(const Nave *nave);
void mostrar_inciso_c(const Nave *nave);
void mostrar_inciso_d(const Nave *nave, double a, double dt);

#endif /* SIMULADOR_H */
