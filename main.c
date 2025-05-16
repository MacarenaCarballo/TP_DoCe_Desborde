#include "DoCe_head.h"
#include "pila_head.h"
#include "cola_head.h"

int main()
{
    char nombre[20];
    int dificultad;
    char opcion;
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
            // Se inicializa el juego
            printf("\n\nFin del juego.\n");
            system("cls");
            break;

        case 'B':

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
