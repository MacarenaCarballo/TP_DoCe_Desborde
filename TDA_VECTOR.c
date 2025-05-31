#include "comun.h"

// Inicializa el vector
void crearVector(tVectorCartas *vec) {
    vec->cantElem = 0;
}

// Inserta una carta al final del vector
int insVecAlFinal(tVectorCartas *vec, signed char carta) {
    if (vec->cantElem == MAX_CARTAS)
        return SIN_MEM;

    vec->datos[vec->cantElem++] = carta;
    return REALIZADO;
}

int verCartaPorPos(const tVectorCartas *vec, unsigned pos, signed char *carta)
{
    if (pos >= CANT_CARTAS || vec->datos[pos] == VACIO)
        return ERROR;

    *carta = vec->datos[pos];
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

int elimPorPosVec(tVectorCartas *vec, unsigned pos, signed char *cartaEliminada) {
    if (!vec || pos >= vec->cantElem || !cartaEliminada)
        return ERROR; // Error por posición inválida o puntero nulo

    *cartaEliminada = vec->datos[pos]; // Guardar la carta eliminada

    // Desplazar los elementos para tapar el hueco
    for (unsigned i = pos; i < vec->cantElem - 1; ++i)
        vec->datos[i] = vec->datos[i + 1];

    vec->cantElem--; // Actualizar cantidad de elementos

    return REALIZADO; // Éxito
}

void vaciarVector(tVectorCartas *vec)
{
    vec->cantElem = 0;
}
