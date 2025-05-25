#ifndef PILA_HEAD_H_INCLUDED
#define PILA_HEAD_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM 320 // Calculo de 40 cartas * ( 4 bytes del dato Int + 4 bytes del tamDelDato unsigned) = 320 bytes
#define PILA_LLENA 1
#define PILA_VACIA 2
#define PILA_NO_VACIA 0
#define PILA_DISPONIBLE 0
#define SATISFACTORIO 0
#define MINIMO(x, y) ((x) <= (y) ? (x) : (y))

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
