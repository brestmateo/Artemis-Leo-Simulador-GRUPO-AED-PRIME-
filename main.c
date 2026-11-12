/*
 * main.c
 * Programa principal: SOLO el menú y el flujo general.
 * Los cálculos están en simulador.c y la lectura segura en validaciones.c.
 */
#include <stdio.h>
#include "simulador.h"
#include "validaciones.h"
#include "analisis.h"
#include "juego.h"


static void mostrar_menu(const Nave *nave)
{
    printf("\n==========================================\n");
    printf("   ARTEMIS - Órbita terrestre baja (LEO)\n");
    printf("==========================================\n");
    printf("  Órbita actual: h = %.1f km | masa = %.1f kg\n",
           nave->orbita.altitud / 1000.0, nave->masa);
    printf("------------------------------------------\n");
    printf("  1) Configurar altitud y masa de la nave\n");
    printf("  2) Inciso a: velocidad orbital\n");
    printf("  3) Inciso b: período orbital\n");
    printf("  4) Inciso c: aceleración centrípeta\n");
    printf("  5) Inciso d: pulso lateral del propulsor\n");
    printf("  6) Inciso e: energía cinética, potencial y mecánica\n");
    printf("  7) Inciso f: gravedad en órbita e ingravidez\n");
    printf("  8) Inciso g: recorrido y ángulo en un tiempo\n");
    printf("  9) Inciso h: Kepler y velocidad de escape\n");
    printf(" 10) Inciso i: órbita resultante tras el pulso\n");
    printf(" 11) Inciso j: energía para cambiar de altitud\n");
    printf(" 12) Inciso k: órbita geoestacionaria\n");
    printf(" 13) Resumen completo\n");
    printf(" 14) JUGAR: Misión de corrección orbital\n");
    printf("  0) Salir\n");
    printf("------------------------------------------\n");
}

static void configurar_nave(Nave *nave)
{
    double h_km, masa;

    printf("\n--- Configuración ---\n");
    h_km = leer_flotante("  Altitud en km [160 a 2000]: ",
                         H_MIN / 1000.0, H_MAX / 1000.0);
    masa = leer_flotante("  Masa de la nave en kg [1000 a 1000000]: ",
                         MASA_MIN, MASA_MAX);

    inicializar_nave(nave, h_km * 1000.0, masa);
    printf("  Configuración actualizada.\n");
}

static void pulso_lateral(const Nave *nave)
{
    double a, dt;

    printf("\n--- Pulso lateral ---\n");
    a = leer_flotante("  Aceleración en m/s^2 [0.01 a 10]: ", 0.01, A_PULSO_MAX);
    dt = leer_flotante("  Duración en s [0.1 a 600]: ", 0.1, DT_PULSO_MAX);
    mostrar_inciso_d(nave, a, dt);
}

static void recorrido_en_tiempo(const Nave *nave)
{
    double t_min = leer_flotante("  Tiempo en minutos [0.1 a 1440]: ", 0.1, 1440.0);
    mostrar_inciso_g(nave, t_min);
}

static void orbita_tras_pulso(const Nave *nave)
{
    double a, dt;

    printf("\n--- Órbita tras el pulso ---\n");
    a = leer_flotante("  Aceleración en m/s^2 [0.01 a 10]: ", 0.01, A_PULSO_MAX);
    dt = leer_flotante("  Duración en s [0.1 a 600]: ", 0.1, DT_PULSO_MAX);
    mostrar_inciso_i(nave, a, dt);
}

static void cambio_de_altitud(const Nave *nave)
{
    double h_km = leer_flotante("  Altitud destino en km [160 a 2000]: ",
                                H_MIN / 1000.0, H_MAX / 1000.0);
    mostrar_inciso_j(nave, h_km * 1000.0);
}

int main(void)
{
    Nave orion;
    int opcion;

    /* Arranca con los datos del enunciado. */
    inicializar_nave(&orion, H_DEFECTO, M_ORION);

    do {
        mostrar_menu(&orion);
        opcion = leer_entero("Elija una opción: ", 0, 14);

        switch (opcion) {
        case 1:  configurar_nave(&orion);          break;
        case 2:  mostrar_inciso_a(&orion);         break;
        case 3:  mostrar_inciso_b(&orion);         break;
        case 4:  mostrar_inciso_c(&orion);         break;
        case 5:  pulso_lateral(&orion);            break;
        case 6:  mostrar_inciso_e(&orion);         break;
        case 7:  mostrar_inciso_f(&orion);         break;
        case 8:  recorrido_en_tiempo(&orion);      break;
        case 9:  mostrar_inciso_h(&orion);         break;
        case 10: orbita_tras_pulso(&orion);        break;
        case 11: cambio_de_altitud(&orion);        break;
        case 12: mostrar_inciso_k(&orion);         break;
        case 13:
            mostrar_inciso_a(&orion);
            mostrar_inciso_b(&orion);
            mostrar_inciso_c(&orion);
            mostrar_inciso_d(&orion, A_PULSO, DT_PULSO);
            break;
        case 14: jugar_mision(&orion);             break;    
        case 0:
            printf("\nFin de la simulación. ¡Buen viaje, Orion!\n");
            break;
        }
    } while (opcion != 0);

    return 0;
}
