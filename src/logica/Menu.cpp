#include "Menu.h"
#include <cstring>

using namespace std;

Menu::Menu()
{
    strcpy_s(opciones[0], "Jugar");
    strcpy_s(opciones[1], "Ranking");
    strcpy_s(opciones[2], "Instrucciones");
    strcpy_s(opciones[3], "Salir");

    opcionSeleccionada = 0;
    numeroOpciones = 4;
}

void Menu::moverArriba()
{
    if (opcionSeleccionada > 0)
    {
        opcionSeleccionada--;
    }
}

void Menu::moverAbajo()
{
    if (opcionSeleccionada < numeroOpciones - 1)
    {
        opcionSeleccionada++;
    }
}

void Menu::mostrarMenu()
{
    int i;

    cout << "==============================" << endl;
    cout << "      ARCHON FOOTBALL         " << endl;
    cout << "==============================" << endl;
    cout << endl;

    for (i = 0; i < numeroOpciones; i++)
    {
        if (i == opcionSeleccionada)
        {
            cout << " > " << opciones[i] << endl;
        }
        else
        {
            cout << "   " << opciones[i] << endl;
        }
    }

    cout << endl;
    cout << "Pulsa w para subir, s para bajar y e para elegir." << endl;
}

int Menu::obtenerOpcionSeleccionada()
{
    return opcionSeleccionada;
}
