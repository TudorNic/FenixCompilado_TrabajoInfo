#pragma once

#include <SFML/Graphics.hpp>
#include <optional>
#include <string>

class PantallaInstrucciones
{
private:
    sf::RenderWindow& ventana;

    sf::Font fuente;
    sf::Text titulo;
    sf::Text textoInstrucciones;
    sf::Text textoVolver;

    bool volverAlMenu;

    float desplazamientoVertical;
    float velocidadScroll;
    float limiteInferiorScroll;

public:
    PantallaInstrucciones(sf::RenderWindow& v, const std::string& rutaFuente);

    void procesarEventos();
    void actualizar();
    void dibujar();

    bool debeVolverAlMenu() const;
    void reiniciarVolver();

private:
    void inicializarTextos();
};

