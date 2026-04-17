#pragma once

#include <SFML/Graphics.hpp>
#include <optional>
#include <string>
#include "Menu.h"
#include "PelotasAnimadas.h"

class PantallaMenu
{
private:
    Menu menu;
    sf::RenderWindow& ventana;

    sf::Font fuente;
    sf::Text titulo;
    sf::Text opcionesTexto[4];

    sf::Texture texturaFondo;
    sf::Sprite fondoCampo;

    PelotaAnimada pelota1;
    PelotaAnimada pelota2;

    bool opcionConfirmada;

public:
    PantallaMenu(sf::RenderWindow& v, const std::string& rutaFuente, const std::string& rutaCampo, const std::string& rutaPelota);

    void procesarEventos();
    void actualizar();
    void dibujar();

    Menu& obtenerMenu();
    bool estaOpcionConfirmada() const;
    Menu::Opcion obtenerOpcionConfirmada() const;
    void reiniciarConfirmacion();

private:
    void inicializarTextos();
    void actualizarAspectoOpciones();
    void manejarTeclado(const sf::Event& evento);
    void manejarRaton(const sf::Event& evento);
    void seleccionarOpcionConRaton(sf::Vector2i posicionRaton);

    std::string obtenerTextoOpcion(int indice) const;
};