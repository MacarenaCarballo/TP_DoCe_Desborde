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

int decoficarCarta(int carta)
{
    switch (carta)
    {
        case MAS_UNO:
        {
            printf("Suma 1\n");
            return 1;
        }
        case MAS_DOS:
        {
            printf("Suma 2\n");
            return 2;
        }
        case SACAR_UNO:
        {
            printf("Saca 1\n");
            return -1;
        }
        case SACAR_DOS:
        {
            printf("Saca 2\n");
            return -2;
        }
        case REPETIR_TURNO:
        {
            printf("Repite turno\n");
            return 3;
        }
        case ESPEJO:
        {
            printf("Espejo\n");
            return 4;
        }
    }
    return 0;
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
        fprintf(pInforme,"NUMERO DE TURNO:%d",jugada.numTurno);
        fprintf(pInforme,"Carta Jugada por %s: %s\n",nombreJugador,jugada.cartaJugador);
        fprintf(pInforme,"Carta Jugada por %s: %s\n",maq,jugada.cartaMaquina);
        fprintf(pInforme,"Puntos Acumulados por %s: %d\n",nombreJugador,jugada.puntosJugador);
        fprintf(pInforme,"Puntos Acumulados por %s: %d\n",maq,jugada.puntosMaquina);

    }
    fprintf(pInforme,"EL GANADOR DEL JUEGO ES: %s\n", ganador==5? nombreJugador: "MAQUINA");

    fclose(pInforme);
    return REALIZADO;

}
