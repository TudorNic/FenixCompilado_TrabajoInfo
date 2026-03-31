//ARCHIVO DE PRUEBA PARA EL MENU, NO ES EL MAIN DEFINITIVO

#include <iostream>
#include "Menu.h"

using namespace std;

int main()
{
    Menu menu;
    char tecla;
    int salir = 0;

    while (salir == 0)
    {
        system("cls");
        menu.mostrarMenu();

        cin >> tecla;

        if (tecla == 'w' || tecla == 'W')
        {
            menu.moverArriba();
        }
        else if (tecla == 's' || tecla == 'S')
        {
            menu.moverAbajo();
        }
        else if (tecla == 'e' || tecla == 'E')
        {
            system("cls");

            if (menu.obtenerOpcionSeleccionada() == 0)
            {
                cout << "Has elegido: Jugar" << endl;
            }
            else if (menu.obtenerOpcionSeleccionada() == 1)
            {
                cout << "Has elegido: Ranking" << endl;
            }
            else if (menu.obtenerOpcionSeleccionada() == 2)
            {
                cout << "Has elegido: Instrucciones" << endl;
            }
            else if (menu.obtenerOpcionSeleccionada() == 3)
            {
                cout << "Has elegido: Salir" << endl;
                salir = 1;
            }

            if (salir == 0)
            {
                cout << endl;
                cout << "Pulsa una tecla y Enter para volver al menu..." << endl;
                cin >> tecla;
            }
        }
    }

    return 0;
}