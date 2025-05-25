#ifndef DOCE_HEAD_H_INCLUDED
#define DOCE_HEAD_H_INCLUDED

#include "pila_head.h"

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "pila_head.h"
#include <time.h>
#include <string.h>
#include <curl/curl.h>
#include "cola_head.h"


#define MAS_UNO 1
#define MAS_DOS 2
#define SACAR_UNO -1
#define SACAR_DOS -2
#define REPETIR_TURNO 3
#define ESPEJO 4
#define GANO_MAQUINA 5
#define GANO_HUMANO 6
#define REALIZADO 0
#define TAM_NOMBRES 15


typedef struct
{
    int numTurno;
    char cartaMaquina[TAM_NOMBRES];
    char cartaJugador[TAM_NOMBRES];
    int puntosJugador;
    int puntosMaquina;
} tInforme;

typedef struct
{
    char codGrupo[10];
    char urlAPi[40];
}tApi;

int generarMazo(tPila *mazo);
int modoFACIL(int vecCartas[]);
const char* decodificarCarta(int valor);
int jugar(char nombre[], int dificultad);
int generarInforme(tCola *informe, int ganador, char *nombreJugador);
int leerConfiguracion(tApi* configuracion);
int  enviarResultadoAPI(tApi* config, const char* nombre, int gano);
int obtenerRanking(tApi *config);
size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp);
void eliminarRanking(tApi* config);


int generarMazo(tPila *p);
int apilarCartas(tPila *p, int *cartas, int cantidad);

#endif // DOCE_HEAD_H_INCLUDED
