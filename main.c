#include "comun.h"


int main()
{
    char nombre[20];
    int dificultad;
    char opcion;
    unsigned char finDelJuego;


    do
    {
        puts("--------MENU---------\n"
             "[A] Jugar \n"
             "[B] Ver ranking \n"
             "[C] Salir \n"
             "\nIngrese una opcion: \n");
        scanf("%c", &opcion);
        opcion = toupper(opcion);
        fflush(stdin);

        switch(opcion)
        {

        case 'A':
            printf("\nIniciando juego...\n");
            printf("Ingrese el nombre del jugador: ");
            scanf("%s", nombre);
            fflush(stdin);
            printf("\nBienvenido %s\n", nombre);
            do
            {
                printf("Seleccione dificultad:\n"
                    "[1] Facil\n"
                    "[2] Medio\n"
                    "[3] Dificil\n"
                    "\nIngrese una opcion: ");

             scanf("%d", &dificultad);
            } while (dificultad < 1 || dificultad > 3);
            system("cls");

            if(dificultad==1)
                finDelJuego = jugar(nombre, modoFACIL);
            else if(dificultad==2)
                    finDelJuego = jugar(nombre, modoMEDIO);
                 else
                    finDelJuego = jugar(nombre, modoDIFICIL);



            printf("\n\n EL GANADOR DEL JUEDO ES %s", finDelJuego==GANO_HUMANO? nombre: "MAQUINA");
            printf("\n\nFin del juego.\n");
            system("pause");
            system("cls");
            break;
/*
        case 'B':
            tApi config;
            if(leerConfiguracion(&config)!= REALIZADO)
            {
                printf("CODIGO DEL GRUPO: %s\n",config.codGrupo);
                printf("URL DE LA API: %s\n",config.urlAPi);
                }
                else
                    printf("ERROR AL LEER LA CONFIGURACIONES DE LA API\n");

            obtenerRanking(&config);

            break;
    */
        case 'C':
            puts("\nSaliendo...\n");
            break;
        default:
            puts("\nOpcion invalida. Intente de nuevo.\n");
        }

        system("pause");
        system("cls");

    }
    while(opcion != 'C');

//    tInforme jugadas[10];
//    tCola informe;
//    crearCola(&informe);
//    int cartasMaquina[10] = {1, -1, 3, 2, 4, -2, 1, -1, 3, 2};
//    int cartasJugador[10] = {2, 3, -1, 1, -2, 4, 3, -2, 1, 4};
//
//    for (int i = 0; i < 10; i++)
//    {
//        jugadas[i].numTurno = i + 1;
//        strncpy(jugadas[i].cartaMaquina, decodificarCarta(cartasMaquina[i]), TAM_NOMBRES);
//        strncpy(jugadas[i].cartaJugador, decodificarCarta(cartasJugador[i]), TAM_NOMBRES);
//        jugadas[i].puntosJugador = (i + 1) * 2;   // ejemplo de puntaje
//        jugadas[i].puntosMaquina = (i + 1) * 3;   // ejemplo de puntaje
//    }
//
//    // Mostrar las jugadas
//    for (int i = 0; i < 10; i++)
//    {
//        printf("Turno %d:\n", jugadas[i].numTurno);
//        printf("  Maquina: %s, Puntos: %d\n", jugadas[i].cartaMaquina, jugadas[i].puntosMaquina);
//        printf("  Jugador: %s, Puntos: %d\n", jugadas[i].cartaJugador, jugadas[i].puntosJugador);
//        ponerEnCola(&informe,&jugadas[i],sizeof(tInforme));
//    }
//    generarInforme(&informe,5,"RAMIRO");
//    vaciarCola(&informe);


    return 0;
}
