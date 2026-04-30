#include <SFML/Graphics.hpp>
#include "PantallaMenu.h"
#include "PantallaInstrucciones.h"
#include "PantallaRanking.h"
#include "Ranking.h"

int main()
{
    sf::RenderWindow ventana(sf::VideoMode({ 800, 600 }), "Menu Archon");
    ventana.setFramerateLimit(60);

    Ranking ranking("../../../assets/texto/ranking_prueba.txt");
	ranking.cargar();

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
        ventana, ranking,
        "../../../assets/fonts/Bungee-Regular.ttf"
    );

    enum EstadoPantalla
    {
        MENU,
        INSTRUCCIONES,
        RANKING_PANTALLA
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
                    estadoActual = RANKING_PANTALLA;
                }

                pantalla.reiniciarConfirmacion();
            }
        }
        else if (estadoActual == INSTRUCCIONES)
        {
            pantallaInstrucciones.procesarEventos();
            pantallaInstrucciones.actualizar();
            pantallaInstrucciones.dibujar();

            if (pantallaInstrucciones.debeVolverAlMenu())
            {
                pantallaInstrucciones.reiniciarVolver();
                estadoActual = MENU;
            }
        }
        else if (estadoActual == RANKING_PANTALLA)
        {
            pantallaRanking.procesarEventos();
            pantallaRanking.actualizar();
            pantallaRanking.dibujar();

            if (pantallaRanking.debeVolverAlMenu())
            {
                pantallaRanking.reiniciarVolver();
                estadoActual = MENU;
            }
        }
       
    }

    return 0;
}