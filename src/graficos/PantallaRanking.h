#pragma once

#include <SFML/Graphics.hpp>
#include <optional>
#include <string>
#include "Ranking.h"

class PantallaRanking
{
private:
    sf::RenderWindow& ventana;
    Ranking& ranking;

    sf::Font fuente;
    sf::Text titulo;
    sf::Text subtitulo;
    sf::Text rankingTexto;
    sf::Text textoVolver;

    bool volverAlMenu;

public:
    PantallaRanking(sf::RenderWindow& v, Ranking& r, const std::string& rutaFuente);

    void procesarEventos();
    void actualizar();
    void dibujar();

    bool debeVolverAlMenu() const;
    void reiniciarVolver();

private:
    void inicializarTextos();
	void actualizarTextoRanking();
};

