#include <SFML/Graphics.hpp>
#include "PantallaMenu.h"

int main()
{
    sf::RenderWindow ventana(sf::VideoMode({ 800, 600 }), "Menu Archon");
    ventana.setFramerateLimit(60);

    PantallaMenu pantalla(ventana, "../../../assets/fonts/Bungee-Regular.ttf", "../../../assets/images/menu/campo_futbol.png",
        "../../../assets/images/menu/pelota_futbol.png", "../../../assets/audio/menu/mover_opcion.mp3",
        "../../../assets/audio/menu/confirmar_opcion.mp3",
        "../../../assets/audio/menu/salir_menu.mp3",
        "../../../assets/audio/menu/musica_haram_fondo_menu.mp3"
    );
       
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