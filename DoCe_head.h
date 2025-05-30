#ifndef DOCE_HEAD_H_INCLUDED
#define DOCE_HEAD_H_INCLUDED

#include <curl/curl.h>

#include "comun.h"

#define ERROR_ARCH   -5

#define SACAR_DOS    -2
#define SACAR_UNO    -1
#define MAS_UNO       1
#define MAS_DOS       2
#define REPETIR_TURNO 3
#define ESPEJO        4

#define GANO_MAQUINA  5
#define GANO_HUMANO   6

#define CERCA_GANAR   8

#define TAM_NOMBRES   15


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
signed char modoFACIL(tVectorCartas *vecCartas, unsigned char *pjeMaquina, unsigned char *pjeHumano, signed char *jugadaHumano);
signed char modoMEDIO(tVectorCartas *vecCartas, unsigned char *pjeMaquina, unsigned char *pjeHumano, signed char *jugadaHumano);
signed char modoDIFICIL(tVectorCartas *vecCartas, unsigned char *pjeMaquina, unsigned char *pjeHumano, signed char *jugadaHumano);
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
