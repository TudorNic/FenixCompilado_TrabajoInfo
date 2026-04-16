#include "PelotasAnimadas.h"

PelotaAnimada::PelotaAnimada(float radio, sf::Vector2f posicionInicial, sf::Vector2f velocidadInicial)
    : velocidad(velocidadInicial)
{
    forma.setRadius(radio);
    forma.setPosition(posicionInicial);
    forma.setFillColor(sf::Color::White);

    // Para que quede algo mas decorativa
    forma.setOutlineThickness(2.f);
    forma.setOutlineColor(sf::Color::Black);
}

void PelotaAnimada::actualizar(const sf::RenderWindow& ventana)
{
    forma.move(velocidad);

    sf::Vector2f posicion = forma.getPosition();
    float radio = forma.getRadius();
    sf::Vector2u size = ventana.getSize();

    // Rebote horizontal
    if (posicion.x <= 0.f || posicion.x + 2.f * radio >= size.x)
    {
        velocidad.x = -velocidad.x;
    }

    // Rebote vertical
    if (posicion.y <= 0.f || posicion.y + 2.f * radio >= size.y)
    {
        velocidad.y = -velocidad.y;
    }
}

void PelotaAnimada::dibujar(sf::RenderWindow& ventana)
{
    ventana.draw(forma);
}

void PelotaAnimada::setColor(const sf::Color& color)
{
    forma.setFillColor(color);
}
