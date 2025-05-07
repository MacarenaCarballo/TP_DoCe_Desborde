#ifndef DOCE_HEAD_H_INCLUDED
#define DOCE_HEAD_H_INCLUDED

#include "pila_head.h"

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define MAS_UNO 1
#define MAS_DOS 2
#define SACAR_UNO -1
#define SACAR_DOS -2
#define REPETIR_TURNO 3
#define ESPEJO 4
#define GANO_MAQUINA 5
#define GANO_HUMANO 6


int generarMazo(tPila *p);
int agregarCartas(tPila* p, int tipo, int cantidad);

#endif // DOCE_HEAD_H_INCLUDED
