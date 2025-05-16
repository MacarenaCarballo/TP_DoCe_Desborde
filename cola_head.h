#ifndef COLA_HEAD_H_INCLUDED
#define COLA_HEAD_H_INCLUDED


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pila_head.h"

#define SIN_MEM -1
#define COLA_LLENA 1
#define COLA_VACIA 2
#define COLA_NO_VACIA 3
#define COLA_DISPONIBLE 4
#define REALIZADO 0

typedef struct sNodo{
void *dato;
unsigned tam;
struct sNodo *sig;}tNodo;

typedef struct{
tNodo* pri;
tNodo* ult;
}tCola;

void crearCola(tCola *cola);
int ponerEnCola(tCola *cola,const void* dato,unsigned tam);
int sacarDeCola(tCola *cola,void* dato,unsigned tam);
int verPrimero(const tCola *cola,void* dato,unsigned tam);
void vaciarCola(tCola *cola);
int colaVacia(const tCola *cola);
int colaLlena(const tCola *cola, unsigned tam);

#endif // COLA_HEAD_H_INCLUDED
