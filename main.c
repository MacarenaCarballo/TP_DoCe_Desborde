#include "DoCe_head.h"
#include "pila_head.h"

int main()
{
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
