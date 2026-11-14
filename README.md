# Artemis-Leo-Simulador-GRUPO-AED-PRIME-
# Artemis - Simulador de Órbita Terrestre Baja (LEO)

Trabajo Final Integrador - Algoritmos y Estructuras de Datos
UTN Facultad Regional Resistencia - Ingeniería en Sistemas de Información

## 1. Integrantes y comisión

Grupo: AED PRIME
Comisión: k 1.4

| Integrante | Usuario de GitHub |
| Arianna Salomé Fernandez | arizzz777 (Salo) |
| Brest Abib Mateo | brestmateo |
| DAugero Valdez Edgar Nicolas | nicodaugero-ux |
| Brogini Tiago | tiagobrogini9-tech |


## 2. Escenario asignado

Situación-Problema 2: Inserción y estabilidad en órbita terrestre baja (MCU y gravitación)

La nave Orion, de 33.000 kg, se encuentra en una órbita circular a 192 km de altitud
sobre la superficie terrestre. El programa calcula, a partir de los datos que ingresa
el usuario:

- Velocidad orbital (la gravedad actúa como fuerza centrípeta).
- Período orbital en segundos y minutos.
- Aceleración centrípeta.
- Efecto de un pulso lateral del propulsor (desviación y nueva órbita).
- Incisos adicionales: energías, gravedad e ingravidez, Kepler, velocidad de escape,
  cambio de altitud y órbita geoestacionaria.

[Las reglas del juego cuando esté implementado.]

## 3. Funcionamiento

El programa muestra un menú principal. El usuario elige un inciso, el programa pide
los datos necesarios (validando tipo y rango) y muestra el resultado con su fórmula.

Estructura del proyecto:

| Archivo | Responsabilidad |
|---|---|
| `main.c` | Función `main()` y menú principal |
| `simulador.h / simulador.c` | Constantes, tipos (`Orbita`, `Nave`) y cálculos físicos |
| `analisis.h / analisis.c` | Incisos adicionales (e a k) |
| `validaciones.h / validaciones.c` | Lectura segura de enteros, reales y caracteres |
| `Makefile` | Compilación |

## 4. Compilación y ejecución

Requisitos: compilador `gcc` y `make`.

    make
    ./artemis

Para borrar los archivos compilados: `make clean`

## 5. Registro de uso de Inteligencia Artificial

| Herramienta | Tarea | Consulta realizada | Cómo se validó / adaptó |
|---|---|---|---|
| Claude | Revisión del código | Se consultó cómo optimizar el código | Se aplicaron solo las sugerencias que correspondían en base a los incisos agregados y se volvió a compilar y probar |
| Claude | Esqueleto modular del programa (`main.c`, `simulador.c/.h`, `validaciones.c/.h`) | Se pidió un esqueleto modular que compile, con los cálculos de los incisos a a d y lectura segura de datos | Se compiló con `make`, se probó cada inciso a mano y se comprobaron los resultados con las fórmulas físicas |
| Claude | Revisión de la integración de las ramas | Se pasaron los zips de las ramas y se pidió revisar que compile todo junto, que la física sea correcta y que se cumpla la consigna | Se contrastaron las observaciones con el enunciado, se aplicaron las que correspondían y se volvió a compilar y probar|
| Claude | Ciclo de turnos del juego (opción 14, `juego.h` / `juego.c`)|e pidió armar un ciclo de turnos. El código fue generado por Claude| Se leyó y se comprendió el código, se jugó para detectar errores y el Pull Request fue revisado por los compañeros |
| Claude | [tarea] | [consulta] | [validación] |
| Claude | [tarea] | [consulta] | [validación] |
