#include "Menu.h"

Menu::Menu()
{
    opcionSeleccionada = JUGAR;
    numeroOpciones = 4;
}

void Menu::moverArriba()
{
    if (opcionSeleccionada > JUGAR)
    {
        opcionSeleccionada = static_cast<Opcion>(opcionSeleccionada - 1);
    }
}

void Menu::moverAbajo()
{
    if (opcionSeleccionada < SALIR)
    {
        opcionSeleccionada = static_cast<Opcion>(opcionSeleccionada + 1);
    }
}

void Menu::seleccionarOpcion(int indice)
{
    if (indice >= 0 && indice < numeroOpciones)
    {
        opcionSeleccionada = static_cast<Opcion>(indice);
    }
}

Menu::Opcion Menu::obtenerOpcionSeleccionada() const
{
    return opcionSeleccionada;
}

int Menu::obtenerIndiceSeleccionado() const
{
    return static_cast<int>(opcionSeleccionada);
}

int Menu::obtenerNumeroOpciones() const
{
    return numeroOpciones;
}