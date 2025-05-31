#ifndef DOCE_HEAD_H_INCLUDED
#define DOCE_HEAD_H_INCLUDED

//#include <curl/curl.h>

#include "comun.h"


typedef struct
{
    unsigned char numTurno;
    tVectorCartas cartaMaquina;
    tVectorCartas cartaJugador;
    signed char puntosJugador;
    signed char puntosMaquina;
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
signed char elegirCartaHumano(const tEstadoJuego* estado);

#endif // DOCE_HEAD_H_INCLUDED
