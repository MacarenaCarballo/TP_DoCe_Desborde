#ifndef DOCE_HEAD_H_INCLUDED
#define DOCE_HEAD_H_INCLUDED

//#include <curl/curl.h>

#include "comun.h"


#define SACAR_DOS    -2
#define SACAR_UNO    -1
#define MAS_UNO       1
#define MAS_DOS       2
#define REPETIR_TURNO 3
#define ESPEJO        4

#define GANO_MAQUINA  5
#define GANO_HUMANO   6

#define TAM_NOMBRES   15


typedef struct
{
    unsigned char numTurno;
    tVectorCartas cartaMaquina;
    tVectorCartas cartaJugador;
    unsigned char puntosJugador;
    unsigned char puntosMaquina;
} tInforme;

typedef struct
{
    char codGrupo[10];
    char urlAPi[40];
}tApi;

typedef struct {
    unsigned char puntajeHumano;
    unsigned char puntajeMaquina;
    signed char ultimaCartaOponente;
    tVectorCartas* cartas;
} tEstadoJuego;

typedef signed char (*tFuncionElegirCarta)(const tEstadoJuego*);

int generarMazo(tPila *mazo);
signed char modoFACIL(const tEstadoJuego* estadoDeJuego);
signed char modoMEDIO(const tEstadoJuego* estadoDeJuego);
signed char modoDIFICIL(const tEstadoJuego* estadoDeJuego);

const char* decodificarCarta(int valor);
unsigned char jugar(const char *nombre, tFuncionElegirCarta dificultadMaq);
/*
int generarInforme(tCola *informe, int ganador, char *nombreJugador);
int leerConfiguracion(tApi* configuracion);
int  enviarResultadoAPI(tApi* config, const char* nombre, int gano);
int obtenerRanking(tApi *config);
size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp);
void eliminarRanking(tApi* config);
*/

int generarMazo(tPila *p);
void mostrarCartasEnLinea(const tVectorCartas *cartas);
void mostrarTablero(unsigned char puntosHum, unsigned char puntosMaq, const tVectorCartas *cartasHum);
int agregarCartas(tVectorCartas* vec, signed char carta, unsigned char cantidad);
signed char elegirCartaHumano(tVectorCartas* cartasHum);

#endif // DOCE_HEAD_H_INCLUDED
