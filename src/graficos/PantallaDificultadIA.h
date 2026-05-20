#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>

class PantallaDificultadIA
{
public:
    enum Dificultad
    {
        FACIL = 0,
        NORMAL = 1,
        DIFICIL = 2
    };

private:
    sf::RenderWindow& ventana;

    sf::Font fuente;
    sf::Text titulo;
    sf::Text opcionesTexto[3];
    sf::Text textoVolver;

    sf::Texture texturaFondo;
    sf::Sprite fondo;

    sf::SoundBuffer bufferMover;
    sf::Sound sonidoMover;

    int opcionSeleccionada;
    bool opcionConfirmada;
    bool volverAlModoJuego;

public:
    PantallaDificultadIA(sf::RenderWindow& v,
        const std::string& rutaFuente,
        const std::string& rutaFondo,
        const std::string& rutaMover);

    void procesarEventos();
    void actualizar();
    void dibujar();

    bool estaOpcionConfirmada() const;
    Dificultad obtenerDificultadConfirmada() const;
    void reiniciarConfirmacion();

    bool debeVolverAlModoJuego() const;
    void reiniciarVolver();

private:
    void inicializarTextos();
    void actualizarAspectoOpciones();
    void moverArriba();
    void moverAbajo();

    void manejarRaton(const sf::Event& evento);
    void seleccionarOpcionConRaton(sf::Vector2i posicionRaton);
};

