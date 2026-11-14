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

Juego: "Misión de corrección orbital"

Orion da vueltas a la Tierra en una órbita circular. En cada turno ocurre un evento aleatorio que lo empuja y deforma su órbita, y el jugador corrige con combustible limitado.

Reglas

La misión dura 5 turnos.
Antes de empezar se elige la dificultad:
Dificultad	| Combustible inicial  |	Fuerza de los golpes  |    	Ayuda
1 - Fácil	  |     250 m/s          |    	10 a 35 m/s       | 	Pistas sobre qué acción conviene
2 - Normal  |     150 m/s	         |     10 a 50 m/s	      |         -
3 - Difícil	|      90 m/s	         |     10 a 70 m/s        |       	-

El combustible se mide en m/s de cambio de velocidad disponibles.
En cada turno ocurre un evento al azar entre 6 posibles: micrometeorito (empuja hacia la Tierra), ráfaga solar (empuja hacia el espacio), atmósfera tenue (frena), falla de propulsor (acelera), basura espacial (frena y empuja hacia abajo) o calma (no pasa nada).
Después del evento el programa muestra cómo quedó la órbita: perigeo (punto más bajo), apogeo (punto más alto), excentricidad y el margen antes de la altura mínima segura.
El jugador elige una acción (una por turno): A acelerar, F frenar, S subir (empujar hacia el espacio), B bajar (empujar hacia la Tierra) o N no hacer nada. Si corrige, indica cuánto cambio de velocidad usa (mínimo 1 m/s, hasta el combustible que le queda).
Se pierde si el perigeo queda por debajo de 150 km de altura (la nave se quema en la atmósfera). Si la nave llegara a escapar de la Tierra, también falla la misión.
Se gana si se sobreviven los 5 turnos.

Puntaje (solo si se gana)

puntos = 500 + 2 x (combustible que sobra) + (1 - excentricidad final) x 500

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
| `juego.h / juego.c` |	Juego por turnos: tabla de eventos, estado de la partida, historial de turnos, puntaje y reporte final.
| `Makefile` | Compilación |

Requisitos técnicos y dónde se cumplen
Estructuras condicionales y repetitivas	if/else, switch, do-while del menú, for: En los turnos.
Modularización: Funciones de una sola responsabilidad separadas en módulos .c/.h.
Arreglos y punteros: Tabla EVENTOS[] y arreglo historial[] de structs; funciones que reciben Nave *, Partida * o devuelven resultados por puntero.
Validaciones exhaustivas: Módulo validaciones: ver 3.3.
Manejo de errores e interfaz limpia: Mensajes [ERROR] y [AVISO] uniformes; el programa nunca se cierra por una entrada inválida.
Control de versiones

El trabajo se desarrolló en un repositorio de GitHub con ramas por componente (feature/...) integradas a main mediante Pull Requests. El historial de commits muestra el desarrollo gradual y la participación de todos los integrantes.

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
| Claude | Reordenar el menú y el switch de main.c. | Pregunté cómo dejar “Resumen completo” al final del menú".| Cambié los números y los case yo mismo y verifiqué que coincidieran. |
| Claude | Revisar main.c y ayudar a compilar. | Le pasé mi código para ver errores y cómo correrlo en VS Code. | Borré el código duplicado, instalé gcc y recompilé después de cada cambio |
