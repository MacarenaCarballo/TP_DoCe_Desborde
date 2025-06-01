#ifndef COLA_HEAD_H_INCLUDED
#define COLA_HEAD_H_INCLUDED

#include "comun.h"

typedef struct sNodo{
void *dato;
unsigned tam;
struct sNodo *sig;}tNodo;

typedef struct {
    tNodo* pri;
    tNodo* ult;
} tCola;

void crearCola(tCola *cola);
int ponerEnCola(tCola *cola,const void* dato,unsigned tam);
int sacarDeCola(tCola *cola,void* dato,unsigned tam);
int verPrimero(const tCola *cola,void* dato,unsigned tam);
void vaciarCola(tCola *cola);
int colaVacia(const tCola *cola);
int colaLlena(const tCola *cola, unsigned tam);

#endif // COLA_HEAD_H_INCLUDED
