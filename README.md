# DoCe_Desborde

**DoCe** es un juego de cartas por turnos entre un jugador humano y una computadora. El objetivo del juego es llegar a los **12** puntos antes que el oponente.

## **¿Cómo jugar?**

- Ejecutar el programa.
- Se mostrará el **MENU** con las diferentes opciones del juego:

### [A] Jugar
1. Ingrese el nombre del jugador.
2. Seleccione el nivel de dificultad:
	- **[1] Facil** : la maquina juega cartas al azar.
	- **[2] Medio** : la maquina evita jugadas inútiles y busca sumar si esta cerca de ganar.
	- **[3] Dificil** : la maquina analiza el estado del juego para elegir la mejor jugada.
3. Se inicia la partida, se comienza con una mano de **3 cartas** por jugador.

Partida de juego:
* En cada turno el jugador seleccionará una carta de su mano y al finalizar su jugada, robará una nueva carta del mazo.

### [B] Ver ranking
Se mostrará la lista de mejores jugadores en base a partidas anteriores.

### [C] Salir
Finaliza el programa.

---

## **Reglas del juego**

- Todos comienzan con **0** puntos y una mano de **3** cartas.
- **No** se puede tener puntos negativos.
- Gana el primero que llegue a **12** puntos.
- Las cartas tienen los siguientes efectos:
	- `MAS DOS` : suma 2 puntos al jugador.
	- `MAS UNO` : suma 1 punto al jugador.
	- `SACAR UNO` : el oponente pierde 1 punto.
	- `SACAR DOS` : el oponente pierde 2 puntos.
	- `REPETIR` : jugas un turno adicional inmediatamente.
	- `ESPEJO` : anula el efecto negativo recibido y lo devuelve al oponente.

---

Trabajo Practico de Algoritmos y Estructuras de datos. Primer Cuatrimestre 2025
