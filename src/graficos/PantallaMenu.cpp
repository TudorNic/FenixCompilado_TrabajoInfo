#include "PantallaMenu.h"
#include <iostream>

using namespace std;

PantallaMenu::PantallaMenu()
{
}

void PantallaMenu::procesarEntrada()
{
    // Aqui ira la captura de teclado y raton con SFML.
    // Por ejemplo:
    // - Flecha arriba  -> menu.moverArriba();
    // - Flecha abajo   -> menu.moverAbajo();
    // - Enter o click  -> confirmar opcion
}

void PantallaMenu::actualizar()
{
    // Aqui ira la logica de actualizacion visual del menu.
    // Por ejemplo:
    // - animaciones
    // - transiciones
    // - efecto hover del raton
}

void PantallaMenu::dibujar()
{
    // Aqui ira el dibujado real con SFML.
    // De momento dejamos una salida sencilla de prueba.

    cout << "===== MENU =====" << endl;
    cout << "Opcion seleccionada: " << menu.obtenerIndiceSeleccionado() << endl;
}

Menu& PantallaMenu::obtenerMenu()
{
    return menu;
}