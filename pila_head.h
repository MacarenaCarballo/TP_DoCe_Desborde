#ifndef PILA_HEAD_H_INCLUDED
#define PILA_HEAD_H_INCLUDED

#include "comun.h"

#define TAM 200 // Calculo de 40 cartas * ( 1 byte del dato unsigned int + 4 bytes del tamDelDato unsigned) = 200 bytes

typedef struct {
char pila[TAM];
int tope;}tPila;

void crearPila(tPila* pila);
int apilar(tPila *pila,const void *dato, unsigned tam);
int desapilar(tPila *pila, void * dato, unsigned tam);
void vaciarPila(tPila *pila);
int verTope(const tPila *pila, void *dato, unsigned tam);
int pilaLlena(const tPila *pila, unsigned tam);
int pilaVacia(const tPila *pila);

#endif // PILA_HEAD_H_INCLUDED
