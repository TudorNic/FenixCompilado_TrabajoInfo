//ARCHIVO DE PRUEBA PARA EL MENU, NO ES EL MAIN DEFINITIVO
#include <conio.h> //libreria para captura de teclado sin necesidad de pulsar enter
#include <iostream>
#include "PantallaMenu.h"

using namespace std;

int main()
{
    PantallaMenu pantalla;
    char tecla;
    int salir = 0;

    while (salir == 0)
    {
        system("cls");
        pantalla.dibujar();

        cout << "Pulsa w para subir, s para bajar, e para salir: ";
		tecla = _getch();  

        if (tecla == 'w' || tecla == 'W')
        {
            pantalla.obtenerMenu().moverArriba();
        }
        else if (tecla == 's' || tecla == 'S')
        {
            pantalla.obtenerMenu().moverAbajo();
        }
        else if (tecla == 'e' || tecla == 'E')
        {
            salir = 1;
        }
    }

    return 0;
}