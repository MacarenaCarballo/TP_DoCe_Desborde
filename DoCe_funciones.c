#include "DoCe_head.h"

int agregarCartas(tPila* p, int tipo, int cantidad) {
    for (int i = 0; i < cantidad; i++) {
        if (apilar(p, &tipo, sizeof(int)) != SATISFACTORIO)
            return PILA_LLENA;
    }
    return SATISFACTORIO;
}

int generarMazo(tPila *p) {

    if (agregarCartas(p, MAS_DOS, 6) != SATISFACTORIO) return PILA_LLENA;
    if (agregarCartas(p, MAS_UNO, 10) != SATISFACTORIO) return PILA_LLENA;
    if (agregarCartas(p, SACAR_UNO, 8) != SATISFACTORIO) return PILA_LLENA;
    if (agregarCartas(p, SACAR_DOS, 6) != SATISFACTORIO) return PILA_LLENA;
    if (agregarCartas(p, REPETIR_TURNO, 6) != SATISFACTORIO) return PILA_LLENA;
    if (agregarCartas(p, ESPEJO, 4) != SATISFACTORIO) return PILA_LLENA;

    return SATISFACTORIO;
}
