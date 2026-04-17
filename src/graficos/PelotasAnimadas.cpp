#include "PelotasAnimadas.h"
#include <stdexcept>
#include<iostream>

PelotaAnimada::PelotaAnimada(const std::string& rutaImagen, sf::Vector2f posicionInicial, sf::Vector2f velocidadInicial, float escala)
    : textura(),sprite(textura), velocidad(velocidadInicial)
{
    if (!textura.loadFromFile(rutaImagen))
    {
        throw std::runtime_error("No se pudo cargar la imagen de la pelota.");
    }

	std::cout << "Pelota cargada correctamente. Tamano: " << textura.getSize().x << " x " << textura.getSize().y << std::endl;//prueba para verificar que la imagen se carga correctamente

	sprite.setTexture(textura,true);
    sprite.setPosition(posicionInicial);
    sprite.setScale({ escala, escala });

    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin({ bounds.size.x / 2.f, bounds.size.y / 2.f });
}

void PelotaAnimada::actualizar(const sf::RenderWindow& ventana)
{
    sprite.move(velocidad);
    sprite.rotate(sf::degrees(0.3f));

    sf::Vector2f posicion = sprite.getPosition();
    sf::FloatRect bounds = sprite.getGlobalBounds();
    sf::Vector2u size = ventana.getSize();

    if (posicion.x - bounds.size.x / 2.f <= 0.f || posicion.x + bounds.size.x / 2.f >= size.x)
    {
        velocidad.x = -velocidad.x;
    }

    if (posicion.y - bounds.size.y / 2.f <= 0.f || posicion.y + bounds.size.y / 2.f >= size.y)
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
    sprite.setRotation(sf::degrees(angulo));
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
    return bounds.size.x / 2.f;
}
