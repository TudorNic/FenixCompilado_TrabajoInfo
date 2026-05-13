#include "PelotasAnimadas.h"
#include <stdexcept>
#include<iostream>

PelotaAnimada::PelotaAnimada(const std::string& rutaImagen, sf::Vector2f posicionInicial, sf::Vector2f velocidadInicial, float escala)
    : velocidad(velocidadInicial)
{
    if (!textura.loadFromFile(rutaImagen))
    {
        throw std::runtime_error("No se pudo cargar la imagen de la pelota.");
    }

	sprite.setTexture(textura,true);
    sprite.setPosition(posicionInicial.x, posicionInicial.y);
    sprite.setScale(escala, escala);

    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f );
}

void PelotaAnimada::actualizar(const sf::RenderWindow& ventana)
{
    sprite.move(velocidad);
    sprite.rotate(0.3f);

    sf::Vector2f posicion = sprite.getPosition();
    sf::FloatRect bounds = sprite.getGlobalBounds();
    sf::Vector2u size = ventana.getSize();

    if (posicion.x - bounds.width / 2.f <= 0.f || posicion.x + bounds.width / 2.f >= static_cast<float>(size.x))
    {
        velocidad.x = -velocidad.x;
    }

    if (posicion.y - bounds.height / 2.f <= 0.f || posicion.y + bounds.height / 2.f >= static_cast<float>(size.y))
    {
        velocidad.y = -velocidad.y;
    }
}

void PelotaAnimada::dibujar(sf::RenderWindow& ventana)
{
    ventana.draw(sprite);
}

void PelotaAnimada::setRotacion(float angulo)
{
    sprite.setRotation(angulo);
}

sf::Vector2f PelotaAnimada::obtenerPosicion() const
{
    return sprite.getPosition();
}

sf::Vector2f PelotaAnimada::obtenerVelocidad() const
{
    return velocidad;
}

void PelotaAnimada::setVelocidad(sf::Vector2f nuevaVelocidad)
{
    velocidad = nuevaVelocidad;
}

float PelotaAnimada::obtenerRadioAproximado() const
{
    sf::FloatRect bounds = sprite.getGlobalBounds();
    return bounds.width / 2.f;
}
