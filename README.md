# Artemis-Leo-Simulador-GRUPO-AED-PRIME-
# Artemis - Simulador de Órbita Terrestre Baja (LEO)

Trabajo Final Integrador - Algoritmos y Estructuras de Datos
UTN Facultad Regional Resistencia - Ingeniería en Sistemas de Información

## 1. Integrantes y comisión

Grupo: AED PRIME
Comisión: k 1.4

| Integrante | Usuario de GitHub |
| Arianna Salomé Fernandez | arizzz777 (Salo) |
| [Nombre y apellido] | [usuario] |
| [Nombre y apellido] | [usuario] |
| [Nombre y apellido] | [usuario] |


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
| Claude | [ej: revisar el código contra la consigna] | [qué se le pidió] | [ej: se compiló y se probó a mano] |
| [otra herramienta] | [tarea] | [consulta] | [validación] |
