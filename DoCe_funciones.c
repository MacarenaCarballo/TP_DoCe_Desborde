#include "DoCe_head.h"
#include "pila_head.h"
#include "cola_head.h"

int generarMazo(tPila *mazo)
{
    int i,num;
    num=MAS_DOS;
    for (i = 0; i < 6; i++)
        apilar(mazo,&num,sizeof(int));

    num=MAS_UNO;
    for (i = 0; i < 10; i++)
        apilar(mazo,&num,sizeof(int));

    num=SACAR_UNO;
    for (i = 0; i < 8; i++)
        apilar(mazo,&num,sizeof(int));

    num=SACAR_DOS;
    for (i = 0; i < 6; i++)
        apilar(mazo,&num,sizeof(int));

    num=REPETIR_TURNO;
    for (i = 0; i < 6; i++)
        apilar(mazo,&num,sizeof(int));

    num=ESPEJO;
    for (i = 0; i < 4; i++)
        apilar(mazo,&num,sizeof(int));

    return REALIZADO;
}

int modoFACIL( int vecCartas[])
{
    srand(time(NULL));

    int numero = rand() % 3;

    return vecCartas[numero];
}

const char* decodificarCarta(int valor)
{
    switch (valor)
    {
        case MAS_UNO: return "MAS_UNO";
        case MAS_DOS: return "MAS_DOS";
        case SACAR_UNO: return "SACAR_UNO";
        case SACAR_DOS: return "SACAR_DOS";
        case REPETIR_TURNO: return "REPETIR_TURNO";
        case ESPEJO: return "ESPEJO";
        default: return "CARTA_DESCONOCIDA";
    }
}

int generarInforme(tCola *informe, int ganador, char *nombreJugador)
{
    tInforme jugada;
    char nombreArchivo[100];
    char *maq="MAQUINA";
    time_t t = time(NULL);
    struct tm* tm_info = localtime(&t); //ESTO ES UNA FUNCION PARA TENER EL DIA ACTUAL CON FECHA Y HORA
    //SALE DE LA LIBRERIA <TIME.H>

    sprintf(nombreArchivo,
        "informe-juego_%04d-%02d-%02d-%02d-%02d.txt",
        tm_info->tm_year + 1900,
        tm_info->tm_mon + 1,
        tm_info->tm_mday,
        tm_info->tm_hour,
        tm_info->tm_min
    );

    FILE* pInforme=fopen(nombreArchivo,"wt");
    if(!pInforme)
    {
        printf("ERROR EN LA APERTURA DEL ARCHIVO INFORME\n");
        return -1;
    }

    fprintf(pInforme,"INFORME DEL JUEGO\n\n");
    while(colaVacia(informe)!=COLA_VACIA)
    {
        sacarDeCola(informe,&jugada,sizeof(tInforme));
        fprintf(pInforme,"NUMERO DE TURNO:%d\n",jugada.numTurno);
        fprintf(pInforme,"Carta Jugada por %s: %s\n",nombreJugador,jugada.cartaJugador);
        fprintf(pInforme,"Carta Jugada por %s: %s\n",maq,jugada.cartaMaquina);
        fprintf(pInforme,"Puntos Acumulados por %s: %d\n",nombreJugador,jugada.puntosJugador);
        fprintf(pInforme,"Puntos Acumulados por %s: %d\n\n",maq,jugada.puntosMaquina);


    }
    fprintf(pInforme,"EL GANADOR DEL JUEGO ES: %s\n", ganador==5? nombreJugador: "MAQUINA");

    fclose(pInforme);
    return REALIZADO;

}

