#ifndef COMUN_H_INCLUDED
#define COMUN_H_INCLUDED

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <string.h>

#include "TDA_VECTOR.h"
#include "pila_head.h"
#include "DoCe_head.h"
#include "cola_head.h"

#define SIN_MEM      -3
#define VACIO        -4
#define ERROR_       -5
#define SACAR_DOS    -2

#define SACAR_UNO    -1

#define REALIZADO     0

#define MAS_UNO       1
#define MAS_DOS       2
#define REPETIR_TURNO 3
#define ESPEJO        4

#define GANO_MAQUINA  5
#define GANO_HUMANO   6
#define CERCA_GANAR   8

#define TAM_NOMBRES   15


#define MINIMO(x, y) ((x) <= (y) ? (x) : (y))

#endif // COMUN_H_INCLUDED
