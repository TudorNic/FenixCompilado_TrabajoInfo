#pragma once
#include <iostream>

class Menu
{
private:
    char opciones[4][30];
    int opcionSeleccionada;
    int numeroOpciones;

public:
    Menu();

    void moverArriba();
    void moverAbajo();
    void mostrarMenu();
    int obtenerOpcionSeleccionada();
};
