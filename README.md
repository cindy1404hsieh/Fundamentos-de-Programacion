# La Sombra de Mac - Juego
![image](https://github.com/cindy1404hsieh/Materias-FIUBA/assets/90338162/4c0fdf33-e965-42a4-94e3-6a60fd88684a)


## Descripción

"La Sombra de Mac" es un juego en el que controlas a un personaje llamado Mac a través de diferentes niveles. El objetivo es acumular puntos, recoger llaves, y avanzar por los niveles evitando obstáculos y enemigos, todo mientras mantienes a Mac con vida. Además, Mac tiene una sombra que también debes cuidar y mover estratégicamente.

## Reglas del Juego

- **Iniciar el Juego**: El juego comienza en el nivel 1.
- **Control del Personaje**: El jugador controla a Mac a través de comandos de teclado.
- **Movimientos de la Sombra**: La sombra de Mac se mueve en sincronía con Mac. La sombra imita los movimientos de Mac, pero puede tener restricciones específicas dependiendo del nivel.
- **Avanzar de Nivel**: Completa el nivel actual para avanzar al siguiente. El juego tiene 3 niveles.
- **Puntos y Vidas**: Acumula puntos y evita perder todas las vidas.
- **Ganar el Juego**: Completa todos los niveles con vida. Tanto Mac como su sombra deben pasar por la puerta con la llave correspondiente.
- **Perder el Juego**: Si Mac pierde todas sus vidas o si la sombra queda atrapada y no puede avanzar, perdes el juego.

## `juego.c`
## Compilación

Para compilar el juego, utiliza el siguiente comando:
```bash
gcc juego.c -o juego -Wall -Wextra -std=c99  
./juego
```


# La Sombra de Mac - Gestión de Partidas

## Descripción

Este programa gestiona las partidas de "La Sombra de Mac". Permite agregar nuevas partidas, eliminar partidas existentes y ordenar las partidas almacenadas en un archivo CSV.

## Funcionalidades

- **Agregar Partida**: Agrega una nueva partida al archivo de registros.
- **Eliminar Partida**: Elimina una partida existente del archivo de registros.
- **Ordenar Partidas**: Ordena las partidas en el archivo de registros por el nombre del jugador.
### `mansion_foster.c`
## Compilación

Para compilar el programa de gestión de partidas, utiliza el siguiente comando:

```bash
gcc mansion_foster.c -o mansion_foster -Wall -Wextra -std=c99
```
## Ejecución
Dependiendo de la operación que desees realizar, utiliza uno de los siguientes comandos:

### Agregar Partida
Para agregar una nueva partida:
```bash
./mansion_foster agregar_partida jugadores.csv
```
### Eliminar Partida
Para eliminar una partida específica (reemplaza player_name con el nombre del jugador):
```bash
./mansion_foster eliminar_partida jugadores.csv "player_name"
```
### Ordenar Partidas
Para ordenar las partidas por el nombre del jugador:
```bash
./mansion_foster ordenar_partida jugadores.csv
```
### Configuración del Juego
Para configurar el juego y ver el estado inicial:
```bash
./mansion_foster config_juego
```
## Durante este trabajo práctico se aplicaron los siguientes conceptos principales:

Programación estructurada.
Manejo de punteros.  
Estructuras de control alternativas y repetitivas.
Datos de tipo simple: entero, real, carácter, lógico, cadena de caracteres.
Constantes y variables.
Operadores aritméticos y sus prioridades.
Procedimientos y funciones, parámetros por valor y por variable.
Variables locales y globales.
Arreglos unidimensionales y bidimensionales.
Métodos de búsqueda y ordenamiento en un array.
Registros, acceso a las componentes, y arreglo de registros.
Archivos: declaración, apertura y tratamiento de archivos secuenciales y de texto.



