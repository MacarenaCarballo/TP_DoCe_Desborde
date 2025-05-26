#include "comun.h"

// Inicializa el vector
void crearVector(tVectorCartas *vec) {
    vec->cantElem = 0;
}

// Inserta una carta al final del vector
int insVecAlFinal(tVectorCartas *vec, signed char carta) {
    if (vec->cantElem >= MAX_CARTAS)
        return SIN_MEM;

    vec->datos[vec->cantElem++] = carta;
    return REALIZADO;
}

// Mezcla el vector (Fisher-Yates)
void mezclarVector(tVectorCartas *vec) {
    for (int i = vec->cantElem - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        signed char tmp = vec->datos[i];
        vec->datos[i] = vec->datos[j];
        vec->datos[j] = tmp;
    }
}

// Extrae una carta del final del vector
int sacarUltVec(tVectorCartas *vec, signed char *carta) {
    if (vec->cantElem == 0)
        return VACIO;

    *carta = vec->datos[--vec->cantElem];
    return REALIZADO;
}

// Acceso por índice (solo lectura)
int verCartaPorPos(const tVectorCartas *vec, unsigned pos, signed char *carta) {
    if (pos >= vec->cantElem)
        return SIN_MEM;

    *carta = vec->datos[pos];
    return REALIZADO;
}

void vaciarVector(tVectorCartas *vec)
{
    vec->cantElem = 0;
}
