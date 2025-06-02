# DoCe_Desborde

**DoCe** es un juego de cartas por turnos entre un jugador humano y una computadora. El objetivo del juego es llegar a los **12** puntos antes que el oponente.

## 🎮**¿Cómo jugar?**

- Ejecutar el programa.
- Se mostrará el **MENU** con las diferentes opciones del juego:

### [A] Jugar
1. Ingrese el nombre del jugador.
2. Seleccione el nivel de dificultad:
	- **[1] Facil**
	- **[2] Medio**
	- **[3] Dificil** 
3. Se inicia la partida, se comienza con una mano de **3 cartas** por jugador.

Partida de juego:
* En cada turno el jugador seleccionará una carta de su mano y al finalizar su jugada, robará una nueva carta del mazo.

### [B] Ver ranking
Se mostrará la lista de mejores jugadores en base a partidas anteriores.

### [C] Salir
Finaliza el programa.

---

## 📝 **Reglas del juego**

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
## ⚙️**Configuración del juego**

El juego cuenta con tres niveles de dificultad:

`Facil` : la maquina juega cartas al azar.
`Medio` : la maquina evita realizar jugadas inefectivas. Si el oponente tiene **0** puntos, no usa cartas de **sacar** puntos y prioriza cartas que suman puntos si esta cerca de ganar.
`Dificil` : la maquina analiza el estado del juego para elegir la mejor jugada. Si el oponente esta cerca de ganar, prioriza cartas de **repetir** turno o **sacar** puntos y usa la carta **espejo** en caso de recibir una carta negativa.

---

Trabajo Practico de Algoritmos y Estructuras de datos. Primer Cuatrimestre 2025
