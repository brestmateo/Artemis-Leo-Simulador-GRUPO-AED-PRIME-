/*
 * analisis.h
 * Misión Artemis - Situación 2: incisos adicionales de análisis (e a k).
 *
 * Usa los tipos y constantes de simulador.h. Cada función mostrar_inciso_X
 * imprime un análisis; las funciones calcular_* hacen UN cálculo y devuelven
 * el resultado (así se pueden probar por separado).
 */
#ifndef ANALISIS_H
#define ANALISIS_H

#include "simulador.h"

/* ---------- Constantes extra ---------- */
#define G_SUPERFICIE   9.80665     /* Gravedad en la superficie [m/s²]          */
#define MASA_ASTRONAUTA 80.0       /* Masa de un astronauta de ejemplo [kg]     */
#define T_SIDEREO      86164.1     /* Día sidéreo (geoestacionaria) [s]         */

/* ---------- Cálculos ---------- */
double calcular_energia_cinetica(double masa, double velocidad);
double calcular_energia_potencial(double masa, double radio);
double calcular_gravedad_en_radio(double radio);
double calcular_velocidad_escape(double radio);
double calcular_periodo_kepler(double semieje);
double calcular_radio_geoestacionario(void);
/* Energía mecánica total que cambia al pasar de r1 a r2 (>0 si se sube). */
double calcular_delta_energia(double masa, double r1, double r2);

/* ---------- Presentación ---------- */
void mostrar_inciso_e(const Nave *nave);                    /* energías              */
void mostrar_inciso_f(const Nave *nave);                    /* gravedad e ingravidez */
void mostrar_inciso_g(const Nave *nave, double t_min);      /* recorrido en t minutos */
void mostrar_inciso_h(const Nave *nave);                    /* Kepler y escape       */
void mostrar_inciso_i(const Nave *nave, double a, double dt);/* órbita tras el pulso  */
void mostrar_inciso_j(const Nave *nave, double h_destino_m); /* cambio de altitud     */
void mostrar_inciso_k(const Nave *nave);                    /* geoestacionaria       */

#endif /* ANALISIS_H */
