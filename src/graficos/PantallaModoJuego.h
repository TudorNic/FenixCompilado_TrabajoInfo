#pragma once

#include <SFML/Graphics.hpp>
#include <optional>
#include <string>

class PantallaModoJuego
{
public:
    enum OpcionModo
    {
        JUGADOR_VS_IA = 0,
        JUGADOR_VS_JUGADOR = 1
    };

private:
    sf::RenderWindow& ventana;

    sf::Font fuente;
    sf::Text titulo;
    sf::Text opcionesTexto[2];
    sf::Text textoVolver;

    int opcionSeleccionada;
    bool opcionConfirmada;
    bool volverAlMenu;

public:
    PantallaModoJuego(sf::RenderWindow& v, const std::string& rutaFuente);

    void procesarEventos();
    void actualizar();
    void dibujar();

    bool estaOpcionConfirmada() const;
    OpcionModo obtenerOpcionConfirmada() const;
    void reiniciarConfirmacion();

    bool debeVolverAlMenu() const;
    void reiniciarVolver();

private:
    void inicializarTextos();
    void actualizarAspectoOpciones();
    void moverArriba();
    void moverAbajo();
};
