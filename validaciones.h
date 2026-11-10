/*
 * validaciones.h
 * Ingreso seguro de datos por consola y mensajes al usuario.
 *
 * Todas las funciones de lectura REPITEN la pregunta hasta que el usuario
 * ingresa un valor válido, así el programa nunca se cuelga ni se cierra
 * por escribir letras donde se esperan números.
 */
#ifndef VALIDACIONES_H
#define VALIDACIONES_H

/* Lee un entero en el rango [min, max]. */
int leer_entero(const char *mensaje, int min, int max);

/* Lee un número real en el rango [min, max]. Acepta coma o punto decimal. */
double leer_flotante(const char *mensaje, double min, double max);

/* Lee UN carácter que esté dentro de 'validos' (en mayúsculas, ej: "SN").
 * Devuelve el carácter en mayúscula. */
char leer_caracter(const char *mensaje, const char *validos);

/* Mensajes con formato uniforme. */
void mostrar_error(const char *mensaje);
void mostrar_advertencia(const char *mensaje);

#endif /* VALIDACIONES_H */
