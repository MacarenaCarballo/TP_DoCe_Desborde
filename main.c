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

        case 'B':
            tApi config;
            if(leerConfiguracion(&config)== REALIZADO)
            {
                printf("CODIGO DEL GRUPO: %s\n",config.codGrupo);
                printf("URL DE LA API: %s\n",config.urlAPi);
                obtenerRanking(&config);
                }
                else
                    printf("ERROR AL LEER LA CONFIGURACIONES DE LA API\n");

            break;

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



    return 0;
}
