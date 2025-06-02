# DOCUMENTACION SOBRE LAS ESTRATEGIAS UTILIZADAS EN DoCe
## 🎯 Objetivo del Trabajo Practico.
 El trabajo práctico consiste en la implementación del juego **DoCe**, aplicando las estructuras de datos, algoritmos vistos en clase. Ademas se agregan conceptos nuevos como la integración de una API externa para consultar un ranking en línea y el uso de repositorios en GitHub para hacer codigo colaborativo.

## 🧱Estructuras de datos utilizadas
### Cola: 
 Utilizamos una cola dinámica para acumular las rondas del juego. En ella se guarda el detalle de cada jugada: qué carta se jugó, quién la jugó y cuántos puntos sumó. Elegimos esta estructura porque permite agregar elementos en orden (FIFO), y al generar el informe final, ya están listados sin necesidad de reordenarlos. 
### Pila: 
 Para representar el mazo de cartas, usamos una pila estática. Elegimos que sea estática porque ya sabemos de antemano cuántas cartas hay, así que no es necesario que se expanda dinámicamente. Y decidimos usar una pila porque es la estructura que más se parece al funcionamiento real de un mazo: las cartas se agregan o se sacan siempre desde el tope, ya que no se puede recorrer la pila.
### Lista Estatica(vector):
 En algunas partes del programa trabajamos con vectores (listas estáticas), por ejemplo para manejar mazos temporales antes de mezclarlos. Esto nos permitió recorrer los elementos, acceder por índice, y reordenarlos para haciendo un "sorteo" manual antes de cargarlos nuevamente en la pila. 

## 🕹️ Estrategia utilizada en el juego
La lógica de juego sigue una estructura por turnos. Cada jugador (humano o máquina) roba una carta del mazo (pila) y ejecuta su jugada. El sistema evalúa la carta, calcula los puntos, y agrega la jugada a la cola de rondas. Durante el desarrollo de la partida se van acumulando los puntos para cada jugador.

### Al inicio del juego:
- Se selecciona **Opciones** del menu con ciertas restricciones(JUGAR, VER RANKING, SALIR).
- Si se selecciona Jugar; se elige dificultad del juego con el cual se llevara a cabo su logica durante toda la partida (FACIL, MEDIO, DIFICIL).

### Durante el juego:
Cada jugador, en su turno:
- Selecciona una carta de su mano (en el caso del jugador humano, elige una por índice).
- Se aplica el efecto de la carta jugada, que puede sumar puntos, restar puntos al oponente, repetir turno, o devolver efectos anteriores.
- El sistema actualiza los puntajes de cada jugador y muestra el estado del tablero.
- La jugada se registra en una cola dinámica (informeJuego) para luego generar el archivo de informe final.
- La carta jugada se envía al vector de descarte.
- Se repone una nueva carta desde la pila del mazo.

En caso de que el mazo se quede sin cartas, se mezclan las cartas del descarte y se reutilizan para seguir el juego. 

### Al finalizar el juego:
- Se determina el ganador comparando puntajes
- Se genera un informe `.txt` con fecha y hora, que incluye todas las jugadas realizadas
- Se envía el resultado a una API REST mediante una solicitud HTTP POST
- El usuario puede consultar el **ranking actualizado** usando un GET a la misma API, el cual se muestra en consola con formato legible


