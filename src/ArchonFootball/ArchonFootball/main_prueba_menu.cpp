#include <SFML/Graphics.hpp>
#include <iostream>
#include "PantallaMenu.h"

int main()
{
    sf::RenderWindow ventana(sf::VideoMode({ 800, 600 }), "Menu Archon");
    ventana.setFramerateLimit(60);

    PantallaMenu pantalla(ventana, "../../../assets/fonts/Bungee-Regular.ttf");

    while (ventana.isOpen())
    {
        pantalla.procesarEventos();
        pantalla.actualizar();
        pantalla.dibujar();

        if (pantalla.estaOpcionConfirmada())
        {
            Menu::Opcion opcion = pantalla.obtenerOpcionConfirmada();

            if (opcion == Menu::SALIR)
            {
                ventana.close();
            }

            pantalla.reiniciarConfirmacion();
        }
    }

    return 0;
}