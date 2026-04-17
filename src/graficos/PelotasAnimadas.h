#pragma once

#include <SFML/Graphics.hpp>
#include <string>

class PelotaAnimada
{
private:
    sf::Texture textura;
    sf::Sprite sprite;
    sf::Vector2f velocidad;

public:
    PelotaAnimada(const std::string& rutaImagen, sf::Vector2f posicionInicial, sf::Vector2f velocidadInicial, float escala);

    void actualizar(const sf::RenderWindow& ventana);
    void dibujar(sf::RenderWindow& ventana);

	void setRotacion(float angulo);

	//Métodos para la deteccion de colisiones
    sf::Vector2f obtenerPosicion() const;
    sf::Vector2f obtenerVelocidad() const;
    void setVelocidad(sf::Vector2f nuevaVelocidad);
    float obtenerRadioAproximado() const;

};
