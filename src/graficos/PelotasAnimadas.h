#pragma once

#include <SFML/Graphics.hpp>

class PelotaAnimada
{
private:
    sf::CircleShape forma;
    sf::Vector2f velocidad;

public:
    PelotaAnimada(float radio, sf::Vector2f posicionInicial, sf::Vector2f velocidadInicial);

    void actualizar(const sf::RenderWindow& ventana);
    void dibujar(sf::RenderWindow& ventana);

    void setColor(const sf::Color& color);
};
