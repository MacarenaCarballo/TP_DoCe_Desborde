#include "DoCe_head.h"
#include "pila_head.h"

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