#ifndef TDA_VECTOR_H_INCLUDED
#define TDA_VECTOR_H_INCLUDED

#include "comun.h"

#define MAX_CARTAS 40

typedef struct {
    signed char datos[MAX_CARTAS]; // -127 a 128 Despues lo cambio
    unsigned cantElem;
} tVectorCartas;

void crearVector(tVectorCartas *vec);
int insVecAlFinal(tVectorCartas *vec,signed char carta);
void mezclarVector(tVectorCartas *vec);
int sacarUltVec(tVectorCartas *vec, signed char *carta);
int verCartaPorPos(const tVectorCartas *vec, unsigned pos, signed char *carta);
void vaciarVector(tVectorCartas *vec);
int elimPorPosVec(tVectorCartas *vec, unsigned pos, signed char *cartaEliminada);

#endif // TDA_VECTOR_H_INCLUDED
