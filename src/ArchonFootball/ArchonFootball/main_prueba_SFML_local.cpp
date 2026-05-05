#include <SFML/Graphics.hpp>
#include <optional>
#include "Arena.h"
#include "Jugador.h"
#include "ControladorIA.h"
#include "Delantero.h"
#include "Defensa.h"
int main()
{
    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "Fenix Football - Test IA");
    window.setFramerateLimit(60);


    Delantero humano;
    Defensa bot;

    // Posiciones iniciales
    humano.setPosicion(100.0f, 300.0f);
    bot.setPosicion(700.0f, 300.0f);

    // Inicializar Arena e IA
    Arena arena(&humano, &bot);
    ControladorIA ia(&bot, &humano, &arena);

    sf::Clock clock;

    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        // Control del Humano (En diagonal se va mas rapido, es solo para testear)
        float hDirX = 0, hDirY = 0;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) hDirY = -1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) hDirY = 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) hDirX = -1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) hDirX = 1;

        arena.comandoMoverJugador1(hDirX, hDirY, deltaTime);


        ia.actualizar(deltaTime);
        arena.actualizar(deltaTime);
        window.clear(sf::Color(34, 139, 34)); // Verde campo

        // Dibujar Humano
        sf::CircleShape shapeH(20.0f);
        shapeH.setFillColor(sf::Color::Blue);
        shapeH.setPosition({ humano.getHitbox().x, humano.getHitbox().y });
        window.draw(shapeH);

        // Dibujar Bot
        sf::CircleShape shapeB(20.0f);
        shapeB.setFillColor(sf::Color::Red);
        shapeB.setPosition({ bot.getHitbox().x, bot.getHitbox().y });
        window.draw(shapeB);

        window.display();
    }

    return 0;
}