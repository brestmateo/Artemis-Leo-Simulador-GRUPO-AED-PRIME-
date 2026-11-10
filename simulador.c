/*
 * simulador.c
 * Lógica física de la simulación: órbita circular (MCU) de la nave Orion.
 *
 * Idea física:
 *   En una órbita circular la ÚNICA fuerza sobre la nave es la gravedad,
 *   y esa fuerza apunta al centro de la Tierra, o sea que hace de
 *   fuerza centrípeta:
 *
 *        F_grav = F_centripeta
 *        G·M·m / r²  =  m·v² / r
 *        v = sqrt( G·M / r )          (la masa m de la nave se cancela)
 */
#include <stdio.h>
#include <math.h>
#include "simulador.h"

/* ---------- Cálculos (cada función hace UNA cosa) ---------- */

double calcular_velocidad_orbital(double radio)
{
    return sqrt(G_GRAV * M_TIERRA / radio);
}

double calcular_periodo(double radio, double velocidad)
{
    /* Una vuelta = longitud de la circunferencia / velocidad. */
    return 2.0 * PI * radio / velocidad;
}

double calcular_aceleracion_centripeta(double velocidad, double radio)
{
    return velocidad * velocidad / radio;
}

double calcular_fuerza_gravitatoria(double masa, double radio)
{
    return G_GRAV * M_TIERRA * masa / (radio * radio);
}

/* 'nave' es un puntero: la función modifica la nave original,
 * no una copia. Se accede a los campos con '->'. */
void inicializar_nave(Nave *nave, double altitud_m, double masa_kg)
{
    nave->masa = masa_kg;
    nave->orbita.altitud = altitud_m;
    nave->orbita.radio = R_TIERRA + altitud_m;
    nave->orbita.velocidad = calcular_velocidad_orbital(nave->orbita.radio);
    nave->orbita.periodo = calcular_periodo(nave->orbita.radio,
                                            nave->orbita.velocidad);
    nave->orbita.acel_centripeta =
        calcular_aceleracion_centripeta(nave->orbita.velocidad,
                                        nave->orbita.radio);
    nave->orbita.fuerza_grav =
        calcular_fuerza_gravitatoria(nave->masa, nave->orbita.radio);
}

/*
 * Pulso lateral: aceleración 'a' constante, perpendicular a la velocidad,
 * durante 'dt' segundos. Se supone que el pulso está en el plano orbital
 * (o sea, apunta en dirección radial).
 *
 *   Δv         = a·Δt              (cambio de velocidad, perpendicular a v)
 *   desviación = ½·a·Δt²           (desplazamiento lateral, parte del reposo lateral)
 *   v_nueva    = sqrt(v² + Δv²)    (Pitágoras: las dos componentes son perpendiculares)
 *
 * Como la velocidad tangencial no cambia y aparece una componente radial,
 * la órbita deja de ser circular y pasa a ser una elipse. Con la
 * energía y el momento angular se obtiene:
 *   e = Δv / v   y   semieje = r / (1 - e²)
 */
ResultadoPulso calcular_pulso_lateral(const Nave *nave, double a, double dt)
{
    ResultadoPulso res;
    double v = nave->orbita.velocidad;
    double r = nave->orbita.radio;

    res.delta_v = a * dt;
    res.desviacion = 0.5 * a * dt * dt;
    res.v_nueva = sqrt(v * v + res.delta_v * res.delta_v);
    res.excentricidad = res.delta_v / v;
    res.semieje = r / (1.0 - res.excentricidad * res.excentricidad);

    return res;
}

/* ---------- Presentación de resultados ---------- */

void mostrar_inciso_a(const Nave *nave)
{
    const Orbita *o = &nave->orbita;
    double fuerza_centripeta = nave->masa * o->velocidad * o->velocidad / o->radio;

    printf("\n=== a) Fuerza gravitatoria como fuerza centrípeta ===\n");
    printf("  Radio orbital r              = %.1f m\n", o->radio);
    printf("  Fuerza gravitatoria G*M*m/r^2 = %.2f N\n", o->fuerza_grav);
    printf("  Fuerza centrípeta   m*v^2/r   = %.2f N\n", fuerza_centripeta);
    printf("  Al igualarlas: G*M*m/r^2 = m*v^2/r  =>  v = sqrt(G*M/r)\n");
    printf("  Velocidad orbital v          = %.2f m/s (%.3f km/s)\n",
           o->velocidad, o->velocidad / 1000.0);
}

void mostrar_inciso_b(const Nave *nave)
{
    const Orbita *o = &nave->orbita;

    printf("\n=== b) Período orbital ===\n");
    printf("  T = 2*pi*r / v\n");
    printf("  T = %.1f s = %.2f min\n", o->periodo, o->periodo / 60.0);
}

void mostrar_inciso_c(const Nave *nave)
{
    const Orbita *o = &nave->orbita;

    printf("\n=== c) Aceleración centrípeta ===\n");
    printf("  a_c = v^2 / r\n");
    printf("  a_c = %.3f m/s^2 (%.1f %% de g en superficie)\n",
           o->acel_centripeta, o->acel_centripeta / 9.80665 * 100.0);
}

void mostrar_inciso_d(const Nave *nave, double a, double dt)
{
    ResultadoPulso p = calcular_pulso_lateral(nave, a, dt);

    printf("\n=== d) Pulso lateral (a = %g m/s^2, dt = %g s) ===\n", a, dt);
    printf("  Cambio de velocidad  dv = a*dt          = %.3f m/s\n", p.delta_v);
    printf("  Desviación lateral   d  = 1/2*a*dt^2    = %.3f m\n", p.desviacion);
    printf("  Nueva rapidez  sqrt(v^2 + dv^2)         = %.4f m/s\n", p.v_nueva);
    printf("  Trayectoria: deja de ser circular y pasa a una elipse\n");
    printf("  Excentricidad e = dv/v                  = %.3e\n", p.excentricidad);
    printf("  Semieje mayor                           = %.2f m\n", p.semieje);
}
