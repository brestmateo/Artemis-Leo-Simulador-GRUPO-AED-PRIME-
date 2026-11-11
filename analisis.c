/*
 * analisis.c
 * Incisos adicionales de la Situación 2 (órbita LEO, MCU y gravitación).
 */
#include <stdio.h>
#include <math.h>
#include "analisis.h"

/* ---------- Cálculos (cada función hace UNA cosa) ---------- */

double calcular_energia_cinetica(double masa, double velocidad)
{
    return 0.5 * masa * velocidad * velocidad;
}

double calcular_energia_potencial(double masa, double radio)
{
    return -G_GRAV * M_TIERRA * masa / radio;   /* cero en el infinito */
}

double calcular_gravedad_en_radio(double radio)
{
    return G_GRAV * M_TIERRA / (radio * radio);
}

double calcular_velocidad_escape(double radio)
{
    return sqrt(2.0 * G_GRAV * M_TIERRA / radio);
}

/* Tercera ley de Kepler: T = 2*pi*sqrt(a^3 / (G*M)). */
double calcular_periodo_kepler(double semieje)
{
    return 2.0 * PI * sqrt(semieje * semieje * semieje / (G_GRAV * M_TIERRA));
}

/* Despejando r de T = 2*pi*sqrt(r^3/(G*M)) con T = día sidéreo. */
double calcular_radio_geoestacionario(void)
{
    double t = T_SIDEREO;
    return cbrt(G_GRAV * M_TIERRA * t * t / (4.0 * PI * PI));
}

double calcular_delta_energia(double masa, double r1, double r2)
{
    /* E = -G*M*m/(2r)  =>  dE = E2 - E1 */
    return -G_GRAV * M_TIERRA * masa / (2.0 * r2)
           + G_GRAV * M_TIERRA * masa / (2.0 * r1);
}

/* ---------- Presentación ---------- */

void mostrar_inciso_e(const Nave *nave)
{
    const Orbita *o = &nave->orbita;
    double k = calcular_energia_cinetica(nave->masa, o->velocidad);
    double u = calcular_energia_potencial(nave->masa, o->radio);

    printf("\n=== e) Energía de la nave en órbita ===\n");
    printf("  Cinética   K = 1/2*m*v^2   = %.4e J\n", k);
    printf("  Potencial  U = -G*M*m/r    = %.4e J\n", u);
    printf("  Mecánica   E = K + U       = %.4e J\n", k + u);
    printf("  Verificación: E = -K       = %.4e J (en órbita circular U = -2K)\n", -k);
    printf("  E < 0: la nave está ligada a la Tierra (no escapa).\n");
}

void mostrar_inciso_f(const Nave *nave)
{
    const Orbita *o = &nave->orbita;
    double g_h = calcular_gravedad_en_radio(o->radio);
    double peso_real = MASA_ASTRONAUTA * g_h;
    double normal = MASA_ASTRONAUTA * (g_h - o->acel_centripeta);

    printf("\n=== f) Gravedad en órbita e ingravidez aparente ===\n");
    printf("  g a esa altitud  g_h = G*M/r^2         = %.3f m/s^2 (%.1f %% de g)\n",
           g_h, g_h / G_SUPERFICIE * 100.0);
    printf("  Astronauta de %.0f kg:\n", MASA_ASTRONAUTA);
    printf("    Peso real (atracción terrestre)      = %.1f N\n", peso_real);
    printf("    Fuerza normal N = m*(g_h - a_c)      = %.3f N\n", normal);
    printf("  La gravedad casi no disminuye: flotan porque la nave y ellos\n");
    printf("  están en caída libre (a_c = g_h), no porque no haya gravedad.\n");
}

void mostrar_inciso_g(const Nave *nave, double t_min)
{
    const Orbita *o = &nave->orbita;
    double t = t_min * 60.0;
    double omega = 2.0 * PI / o->periodo;
    double angulo_rad = omega * t;
    double angulo_grados = angulo_rad * 180.0 / PI;
    double distancia = o->velocidad * t;

    printf("\n=== g) Movimiento angular y recorrido en %g min ===\n", t_min);
    printf("  Velocidad angular w = 2*pi/T        = %.6f rad/s\n", omega);
    printf("  Velocidad lineal    v               = %.1f km/h\n",
           o->velocidad * 3.6);
    printf("  Vueltas por día (24 h)              = %.2f\n", 86400.0 / o->periodo);
    printf("  Ángulo barrido en %g min             = %.1f grados (%.3f vueltas)\n",
           t_min, angulo_grados, angulo_grados / 360.0);
    printf("  Distancia recorrida en %g min        = %.1f km\n",
           t_min, distancia / 1000.0);
}

void mostrar_inciso_h(const Nave *nave)
{
    const Orbita *o = &nave->orbita;
    double t_kepler = calcular_periodo_kepler(o->radio);
    double v_esc = calcular_velocidad_escape(o->radio);

    printf("\n=== h) Tercera ley de Kepler y velocidad de escape ===\n");
    printf("  T^2 / r^3 (nave)                    = %.4e s^2/m^3\n",
           o->periodo * o->periodo / (o->radio * o->radio * o->radio));
    printf("  4*pi^2 / (G*M)  (teórico)           = %.4e s^2/m^3\n",
           4.0 * PI * PI / (G_GRAV * M_TIERRA));
    printf("  Período por Kepler                  = %.1f s (coincide con inciso b)\n",
           t_kepler);
    printf("  Velocidad de escape a esa altura    = %.1f m/s (%.3f km/s)\n",
           v_esc, v_esc / 1000.0);
    printf("  Relación v_escape / v_orbital       = %.4f (= raiz de 2)\n",
           v_esc / o->velocidad);
    printf("  Aumento de rapidez para escapar     = %.1f m/s\n",
           v_esc - o->velocidad);
}

void mostrar_inciso_i(const Nave *nave, double a, double dt)
{
    ResultadoPulso p = calcular_pulso_lateral(nave, a, dt);
    double r_peri = p.semieje * (1.0 - p.excentricidad);
    double r_apo = p.semieje * (1.0 + p.excentricidad);
    double t_nuevo = calcular_periodo_kepler(p.semieje);

    printf("\n=== i) Órbita resultante tras el pulso (a = %g m/s^2, dt = %g s) ===\n",
           a, dt);
    printf("  Excentricidad e                     = %.3e\n", p.excentricidad);
    printf("  Semieje mayor                       = %.2f km\n", p.semieje / 1000.0);
    printf("  Perigeo: altitud                    = %.2f km\n",
           (r_peri - R_TIERRA) / 1000.0);
    printf("  Apogeo:  altitud                    = %.2f km\n",
           (r_apo - R_TIERRA) / 1000.0);
    printf("  Nuevo período                       = %.2f s (antes %.2f s)\n",
           t_nuevo, nave->orbita.periodo);
    if (r_peri <= R_TIERRA) {
        printf("  [PELIGRO] El perigeo queda bajo la superficie: la nave cae a la Tierra.\n");
    } else {
        printf("  La órbita sigue siendo segura (el perigeo está sobre la superficie).\n");
    }
}

void mostrar_inciso_j(const Nave *nave, double h_destino_m)
{
    double r2 = R_TIERRA + h_destino_m;
    double dE = calcular_delta_energia(nave->masa, nave->orbita.radio, r2);
    double v2 = calcular_velocidad_orbital(r2);

    printf("\n=== j) Cambio de altitud: %.1f km -> %.1f km ===\n",
           nave->orbita.altitud / 1000.0, h_destino_m / 1000.0);
    printf("  Velocidad orbital en la nueva órbita = %.2f m/s\n", v2);
    printf("  Variación de energía mecánica dE     = %.4e J\n", dE);
    if (dE > 0.0) {
        printf("  Hay que SUMAR energía (subir): la nave va más lenta pero más alto.\n");
    } else if (dE < 0.0) {
        printf("  La nave PIERDE energía al bajar: va más rápida pero más abajo.\n");
    } else {
        printf("  Misma órbita: no hay cambio de energía.\n");
    }
}

void mostrar_inciso_k(const Nave *nave)
{
    double r_geo = calcular_radio_geoestacionario();
    double v_geo = calcular_velocidad_orbital(r_geo);

    printf("\n=== k) Órbita geoestacionaria (T = día sidéreo) ===\n");
    printf("  Radio orbital                       = %.1f km\n", r_geo / 1000.0);
    printf("  Altitud sobre la superficie         = %.1f km\n",
           (r_geo - R_TIERRA) / 1000.0);
    printf("  Velocidad orbital                   = %.1f m/s\n", v_geo);
    printf("  Altitud actual vs geoestacionaria   = %.1f km vs %.1f km\n",
           nave->orbita.altitud / 1000.0, (r_geo - R_TIERRA) / 1000.0);
}
