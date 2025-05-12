#ifndef DOCE_HEAD_H_INCLUDED
#define DOCE_HEAD_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "pila_head.h"
#include <time.h>
#include <string.h>

#define MAS_UNO 1
#define MAS_DOS 2
#define SACAR_UNO -1
#define SACAR_DOS -2
#define REPETIR_TURNO 3
#define ESPEJO 4
#define GANO_MAQUINA 5
#define GANO_HUMANO 6
#define REALIZADO 0

int generarMazo(tPila *mazo);
int modoFACIL(int vecCartas[]);
int decoficarCarta(int carta);
int jugar(char nombre[], int dificultad);
#endif // DOCE_HEAD_H_INCLUDED
