#include <SFML/Graphics.hpp>
#include "PantallaMenu.h"
#include "PantallaInstrucciones.h"
#include "PantallaRanking.h"

int main()
{
    sf::RenderWindow ventana(sf::VideoMode({ 800, 600 }), "Menu Archon");
    ventana.setFramerateLimit(60);

    PantallaMenu pantalla(ventana, "../../../assets/fonts/Bungee-Regular.ttf", "../../../assets/images/menu/fondo_haram.png",
        "../../../assets/images/menu/pelota_futbol.png", "../../../assets/audio/menu/mover_opcion.mp3",
        "../../../assets/audio/menu/confirmar_opcion.mp3",
        "../../../assets/audio/menu/salir_menu.mp3",
        "../../../assets/audio/menu/musica_haram_fondo_menu.mp3"
    );
    
    PantallaInstrucciones pantallaInstrucciones(
        ventana,
        "../../../assets/fonts/Bungee-Regular.ttf"
    );

    PantallaRanking pantallaRanking(
        ventana,
        "../../../assets/fonts/Bungee-Regular.ttf"
    );

    enum EstadoPantalla
    {
        MENU,
        INSTRUCCIONES,
        RANKING
    };

    EstadoPantalla estadoActual = MENU;

    while (ventana.isOpen())
    {
        if (estadoActual == MENU) 
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
                else if (opcion == Menu::INSTRUCCIONES)
                {
                    estadoActual = INSTRUCCIONES;

                }
                else if (opcion == Menu::RANKING)
                {
                    estadoActual = RANKING;
                }

                pantalla.reiniciarConfirmacion();
            }
        }
        else if (estadoActual == INSTRUCCIONES)
        {
            pantallaInstrucciones.procesarEventos();
            pantallaInstrucciones.actualizar();
            pantallaInstrucciones.dibujar();
        }
        else if (estadoActual == RANKING)
        {
            pantallaRanking.procesarEventos();
            pantallaRanking.actualizar();
            pantallaRanking.dibujar();

            if (pantallaInstrucciones.debeVolverAlMenu())
            {
                pantallaInstrucciones.reiniciarVolver();
                estadoActual = MENU;
            }
        }
    }

    return 0;
}