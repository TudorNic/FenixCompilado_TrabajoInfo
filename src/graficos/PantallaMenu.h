#pragma once

#include "Menu.h"

class PantallaMenu
{
private:
    Menu menu;

public:
    PantallaMenu();

    void procesarEntrada();
    void actualizar();
    void dibujar();

    Menu& obtenerMenu();
};