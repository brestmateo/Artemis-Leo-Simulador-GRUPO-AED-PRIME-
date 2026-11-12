/*
 * juego.c
 * Juego "Misión de corrección orbital" (Situación 2: órbita LEO).
 *
 * Idea física (la misma que en simulador.c / analisis.c):
 *   La nave tiene una velocidad radial (hacia afuera o hacia la Tierra) y
 *   una velocidad tangencial (la de giro). Con esos dos números se calcula
 *   la energía y el momento angular, y de ahí la forma de la órbita:
 *
 *      E  = v^2/2 - G*M/r            (energía por unidad de masa)
 *      h  = r * v_tangencial         (momento angular por unidad de masa)
 *      a  = -G*M / (2E)              (semieje mayor)
 *      e  = sqrt(1 + 2*E*h^2/(G*M)^2) (excentricidad)
 *      perigeo = a*(1-e)    apogeo = a*(1+e)   (distancias al centro)
 *
 *   Un evento suma velocidad en alguna dirección y la órbita se deforma.
 *   El jugador corrige sumando velocidad en la dirección contraria.
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "juego.h"
#include "validaciones.h"

/* Resultados posibles de un turno / de la misión. */
#define RES_OK         0
#define RES_REENTRADA  1
#define RES_ESCAPO     2

/* ---------- Tablas (arreglos) ---------- */

/* Los 6 eventos posibles. Los factores valen 1 (empuje completo), -1
 * (empuje contrario) o 0.7071 (diagonal: mitad en cada dirección). */
const TipoEvento EVENTOS[N_EVENTOS] = {
    { "Un micrometeorito te golpea y te empuja hacia la Tierra.",
      "Micrometeorito",  -1.0,     0.0,     'S' },
    { "Una ráfaga solar te empuja hacia el espacio.",
      "Rafaga solar",     1.0,     0.0,     'B' },
    { "La atmósfera tenue te frena (perdés velocidad).",
      "Atmosfera",        0.0,    -1.0,     'A' },
    { "Un propulsor falla y te acelera sin querer.",
      "Falla propulsor",  0.0,     1.0,     'F' },
    { "Basura espacial te golpea de costado: te frena y te empuja hacia abajo.",
      "Basura espacial", -0.7071, -0.7071,  'A' },
    { "Todo tranquilo: este turno no pasa nada.",
      "Calma",            0.0,     0.0,     'N' }
};

/* Una posición por dificultad (1 fácil, 2 normal, 3 difícil). */
static const double COMBUSTIBLE_INICIAL[3] = { 250.0, 150.0, 90.0 };
static const double MAGNITUD_MAX[3]        = {  35.0,  50.0, 70.0 };
#define MAGNITUD_MIN 10.0

/* ---------- Cálculos ---------- */

void iniciar_partida(Partida *p, const Nave *nave, int dificultad)
{
    if (dificultad < 1) dificultad = 1;
    if (dificultad > 3) dificultad = 3;

    p->radio = nave->orbita.radio;
    p->v_radial = 0.0;                       /* órbita circular al empezar */
    p->v_tang = nave->orbita.velocidad;
    p->combustible = COMBUSTIBLE_INICIAL[dificultad - 1];
    p->combustible_inicial = p->combustible;
    p->dificultad = dificultad;
    p->turnos_jugados = 0;
}

int calcular_orbita_estado(double radio, double v_radial, double v_tang,
                           double *alt_perigeo, double *alt_apogeo,
                           double *excentricidad)
{
    double mu = G_GRAV * M_TIERRA;
    double energia = 0.5 * (v_radial * v_radial + v_tang * v_tang) - mu / radio;
    double h = radio * v_tang;
    double semieje, e2, e;

    if (energia >= 0.0) {
        return 0;                            /* escapó de la Tierra */
    }

    semieje = -mu / (2.0 * energia);
    e2 = 1.0 + 2.0 * energia * h * h / (mu * mu);
    if (e2 < 0.0) {
        e2 = 0.0;                            /* error de redondeo */
    }
    e = sqrt(e2);

    /* Los resultados se devuelven por puntero (se escribe con '*'). */
    *excentricidad = e;
    *alt_perigeo = semieje * (1.0 - e) - R_TIERRA;
    *alt_apogeo = semieje * (1.0 + e) - R_TIERRA;
    return 1;
}

double sortear_magnitud(int dificultad)
{
    double max = MAGNITUD_MAX[dificultad - 1];
    double azar = (double)rand() / (double)RAND_MAX;   /* entre 0 y 1 */

    return MAGNITUD_MIN + (max - MAGNITUD_MIN) * azar;
}

void aplicar_evento(Partida *p, int id, double magnitud)
{
    p->v_radial += EVENTOS[id].factor_radial * magnitud;
    p->v_tang += EVENTOS[id].factor_tangencial * magnitud;
}

int aplicar_correccion(Partida *p, char accion, double dv)
{
    if (accion == 'N') {
        return 1;                            /* no hacer nada siempre se puede */
    }
    if (dv <= 0.0 || dv > p->combustible) {
        return 0;
    }

    switch (accion) {
    case 'A': p->v_tang += dv;   break;      /* acelerar */
    case 'F': p->v_tang -= dv;   break;      /* frenar   */
    case 'S': p->v_radial += dv; break;      /* subir (hacia el espacio) */
    case 'B': p->v_radial -= dv; break;      /* bajar (hacia la Tierra)  */
    default:  return 0;
    }

    p->combustible -= dv;
    return 1;
}

/* ---------- Presentación ---------- */

static const char *nombre_accion(char accion)
{
    switch (accion) {
    case 'A': return "Acelerar";
    case 'F': return "Frenar";
    case 'S': return "Subir";
    case 'B': return "Bajar";
    default:  return "Nada";
    }
}

/* Barra de seguridad: 20 casilleros, llena = lejos del peligro. */
static void dibujar_barra(double margen_km)
{
    int llenos, i;

    llenos = (int)(margen_km / 50.0 * 20.0);
    if (llenos < 0) llenos = 0;
    if (llenos > 20) llenos = 20;

    printf("  [");
    for (i = 0; i < 20; i++) {
        putchar(i < llenos ? '#' : '-');
    }
    printf("]\n");
}

static void mostrar_estado_orbita(const char *titulo, double alt_perigeo,
                                  double alt_apogeo, double excentricidad,
                                  int definitivo)
{
    double margen_km = (alt_perigeo - ALT_MIN_SEGURA) / 1000.0;

    printf("\n  %s:\n", titulo);
    printf("    Punto más bajo de la órbita (perigeo): %.1f km\n",
           alt_perigeo / 1000.0);
    printf("    Punto más alto de la órbita (apogeo):  %.1f km\n",
           alt_apogeo / 1000.0);
    printf("    Forma: excentricidad %.4f (0 = círculo perfecto)\n",
           excentricidad);
    printf("    Margen antes de quemarte (mínimo %.0f km): %.1f km\n",
           ALT_MIN_SEGURA / 1000.0, margen_km);
    dibujar_barra(margen_km);

    if (margen_km < 0.0 && definitivo) {
        mostrar_error("¡REENTRADA! La nave baja de la altura segura.");
    } else if (margen_km < 0.0) {
        mostrar_advertencia("¡ALERTA! Si no corregís, la nave se quema.");
    } else if (margen_km < 15.0) {
        mostrar_advertencia("PELIGRO: estás muy cerca de la atmósfera.");
    } else {
        printf("  Estado: SEGURA\n");
    }
}

static void mostrar_instrucciones(void)
{
    printf("\n==========================================\n");
    printf("   MISIÓN DE CORRECCIÓN ORBITAL\n");
    printf("==========================================\n");
    printf(" Orion da vueltas a la Tierra. Cada turno algo lo\n");
    printf(" golpea y deforma su órbita. Vos corregís con el\n");
    printf(" combustible que tenés (se mide en m/s).\n\n");
    printf(" - Si el punto más bajo de la órbita queda a menos de\n");
    printf("   %.0f km de altura, la nave se quema en la atmósfera.\n",
           ALT_MIN_SEGURA / 1000.0);
    printf(" - Si sobrevivís %d turnos, ganás.\n", N_TURNOS);
    printf(" - Cuanto más combustible te sobre y más redonda sea\n");
    printf("   la órbita, más puntos sumás.\n");
}

static void mostrar_opciones(void)
{
    printf("\n  ¿Qué hacés?\n");
    printf("    A = Acelerar (ir más rápido)\n");
    printf("    F = Frenar\n");
    printf("    S = Subir (empujar hacia el espacio)\n");
    printf("    B = Bajar (empujar hacia la Tierra)\n");
    printf("    N = No hacer nada\n");
    printf("  Consejo: corregí empujando en sentido contrario al golpe.\n");
}

/* ---------- Un turno ---------- */

/* Juega el turno número 'turno' (1, 2, 3...). Devuelve RES_OK si la nave
 * sigue en una órbita segura, o RES_REENTRADA / RES_ESCAPO si se perdió. */
static int jugar_turno(Partida *p, int turno)
{
    int id = rand() % N_EVENTOS;
    double magnitud = sortear_magnitud(p->dificultad);
    double alt_perigeo, alt_apogeo, exc;
    double dv = 0.0;
    char accion = 'N';
    char pregunta[120];
    Maniobra *m = &p->historial[turno - 1];   /* puntero al registro del turno */

    printf("\n------------------ TURNO %d de %d ------------------\n",
           turno, N_TURNOS);
    printf("  Combustible disponible: %.1f m/s\n", p->combustible);

    printf("\n  ¡EVENTO! %s\n", EVENTOS[id].descripcion);
    if (id == N_EVENTOS - 1) {
        magnitud = 0.0;                       /* "Calma": sin efecto */
    } else {
        printf("  (fuerza del golpe: %.1f m/s)\n", magnitud);
    }
    aplicar_evento(p, id, magnitud);

    if (!calcular_orbita_estado(p->radio, p->v_radial, p->v_tang,
                                &alt_perigeo, &alt_apogeo, &exc)) {
        return RES_ESCAPO;
    }
    mostrar_estado_orbita("Así quedó tu órbita", alt_perigeo, alt_apogeo, exc, 0);

    if (p->combustible < DV_MIN_CORRECCION) {
        printf("\n  Te quedaste sin combustible: no podés corregir.\n");
    } else {
        mostrar_opciones();
        if (p->dificultad == 1 && EVENTOS[id].accion_sugerida != 'N') {
            printf("  Pista: probá con '%c'.\n", EVENTOS[id].accion_sugerida);
        } else if (p->dificultad == 1) {
            printf("  Pista: no hace falta corregir (podés arreglar la órbita).\n");
        }
        accion = leer_caracter("  Tu decisión [A/F/S/B/N]: ", "AFSBN");

        if (accion != 'N') {
            snprintf(pregunta, sizeof pregunta,
                     "  ¿Cuánto cambio de velocidad usás? [%.1f a %.1f m/s]: ",
                     DV_MIN_CORRECCION, p->combustible);
            dv = leer_flotante(pregunta, DV_MIN_CORRECCION, p->combustible);
            aplicar_correccion(p, accion, dv);

            calcular_orbita_estado(p->radio, p->v_radial, p->v_tang,
                                   &alt_perigeo, &alt_apogeo, &exc);
            mostrar_estado_orbita("Después de tu corrección",
                                  alt_perigeo, alt_apogeo, exc, 1);
        } else {
            printf("\n  No hiciste nada: la órbita queda como está.\n");
        }
    }

    /* Guardar el turno en el historial. */
    m->turno = turno;
    m->evento = id;
    m->magnitud = magnitud;
    m->accion = accion;
    m->dv_gastado = dv;
    m->perigeo_km = alt_perigeo / 1000.0;
    m->excentricidad = exc;
    p->turnos_jugados = turno;

    if (alt_perigeo < ALT_MIN_SEGURA) {
        return RES_REENTRADA;
    }
    return RES_OK;
}

/* ---------- Reporte final ---------- */

static void mostrar_reporte(const Partida *p, int resultado)
{
    int i, puntos = 0;
    double exc_final = 0.0, maximo;
    const char *rango;

    printf("\n==========================================\n");
    printf("   REPORTE FINAL DE LA MISIÓN\n");
    printf("==========================================\n");
    printf("  Turno  Evento            Acción    Dv usado   Perigeo\n");
    printf("  -----  ----------------  --------  ---------  ----------\n");
    for (i = 0; i < p->turnos_jugados; i++) {
        const Maniobra *m = &p->historial[i];
        printf("  %3d    %-16s  %-8s  %5.1f m/s  %7.1f km\n",
               m->turno, EVENTOS[m->evento].nombre_corto,
               nombre_accion(m->accion), m->dv_gastado, m->perigeo_km);
    }
    printf("  -----  ----------------  --------  ---------  ----------\n");
    printf("  Combustible que sobró: %.1f m/s\n", p->combustible);

    if (resultado == RES_REENTRADA) {
        printf("\n  MISIÓN FALLIDA: la nave se quemó en la atmósfera.\n");
    } else if (resultado == RES_ESCAPO) {
        printf("\n  MISIÓN FALLIDA: la nave se perdió en el espacio.\n");
    } else {
        exc_final = p->historial[p->turnos_jugados - 1].excentricidad;
        if (exc_final > 1.0) exc_final = 1.0;

        puntos = 500 + (int)(p->combustible * 2.0)
                     + (int)((1.0 - exc_final) * 500.0);
        /* El máximo posible depende del combustible con el que se empezó. */
        maximo = 1000.0 + 2.0 * p->combustible_inicial;
        if (puntos >= 0.85 * maximo)      rango = "COMANDANTE";
        else if (puntos >= 0.65 * maximo) rango = "PILOTO EXPERTO";
        else                              rango = "CADETE";

        printf("\n  ¡MISIÓN CUMPLIDA! Orion sigue en órbita.\n");
        printf("  Excentricidad final: %.4f\n", exc_final);
        printf("  PUNTAJE: %d   Rango: %s\n", puntos, rango);
    }
    printf("==========================================\n");
}

/* ---------- Función pública ---------- */

void jugar_mision(const Nave *nave)
{
    Partida partida;
    int dificultad, turno, resultado = RES_OK;

    mostrar_instrucciones();
    printf("\n  Dificultad:\n");
    printf("    1) Fácil    (más combustible, golpes suaves, con pistas)\n");
    printf("    2) Normal\n");
    printf("    3) Difícil  (poco combustible, golpes fuertes)\n");
    dificultad = leer_entero("  Elegí la dificultad [1 a 3]: ", 1, 3);

    srand((unsigned int)time(NULL));          /* para que los eventos varíen */
    iniciar_partida(&partida, nave, dificultad);

    for (turno = 1; turno <= N_TURNOS && resultado == RES_OK; turno++) {
        resultado = jugar_turno(&partida, turno);
    }

    mostrar_reporte(&partida, resultado);
}
