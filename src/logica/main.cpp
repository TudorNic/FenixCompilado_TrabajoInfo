#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <string>

#include "Arena.h"
#include "Defensa.h"
#include "Delantero.h"
#include "Aficion.h"
#include "Entrenador.h"
#include "Centrocampista.h"
#include "Lateral.h"

int main() {
    sf::Texture texC, texB;
    if (!texC.loadFromFile("campo.png") || !texB.loadFromFile("assets/proyectil/balon_proyectil.png")) return -1;

    float anV = (float)texC.getSize().x;
    float alV = (float)texC.getSize().y;
    sf::RenderWindow window(sf::VideoMode((int)anV, (int)alV), "Archon Football - Attack Animation System");
    window.setFramerateLimit(60);

    Jugador* azul = new Entrenador();
    Jugador* rojo = new Defensa();
    Arena arena(azul, rojo);

    // CARGA DINÁMICA
    std::vector<sf::Texture> walkA(3), walkR(3);
    sf::Texture atkA, atkR;
    std::string cA = azul->getNombreClase();
    std::string cR = rojo->getNombreClase();

    for (int i = 0; i < 3; i++) {
        walkA[i].loadFromFile("assets/players/blue/" + cA + "/walk/sprite_" + cA + "_blue_walk-" + std::to_string(i + 1) + ".png");
        walkR[i].loadFromFile("assets/players/red/" + cR + "/walk/sprite_" + cR + "_red_walk-" + std::to_string(i + 1) + ".png");
    }

    // CAMBIO: Ahora carga la imagen 3 de la carpeta "atack"
    atkA.loadFromFile("assets/players/blue/" + cA + "/atack/sprite_" + cA + "_blue_atack-3.png");
    atkR.loadFromFile("assets/players/red/" + cR + "/atack/sprite_" + cR + "_red_atack-3.png");

    sf::Sprite sprA(walkA[0]), sprR(walkR[0]), sprC(texC), sprB(texB);
    sprB.setOrigin(10.f, 10.f);

    float anchoB = 60.0f; float altoB = 7.0f;
    sf::RectangleShape hpBA(sf::Vector2f(anchoB, altoB)), hpFA(sf::Vector2f(anchoB, altoB));
    sf::RectangleShape hpBR(sf::Vector2f(anchoB, altoB)), hpFR(sf::Vector2f(anchoB, altoB));
    hpBA.setFillColor(sf::Color(50, 50, 50)); hpFA.setFillColor(sf::Color::Green);
    hpBR.setFillColor(sf::Color(50, 50, 50)); hpFR.setFillColor(sf::Color::Green);

    int vPA = azul->getVidaActual(); int vPR = rojo->getVidaActual();
    azul->setPosicion(anV * 0.2f, alV * 0.5f);
    rojo->setPosicion(anV * 0.8f, alV * 0.5f);

    sf::Clock clock;
    float mirA = 1.0f, mirR = -1.0f;
    float offY = 40.0f;

    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();
        sf::Event ev;
        while (window.pollEvent(ev)) { if (ev.type == sf::Event::Closed) window.close(); }

        // MOVIMIENTO
        sf::Vector2f mA(0.f, 0.f), mR(0.f, 0.f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) mA.y -= 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) mA.y += 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) { mA.x -= 1; mirA = -1.0f; }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) { mA.x += 1; mirA = 1.0f; }
        if (mA.x != 0 || mA.y != 0) {
            azul->setEstado(CAMINANDO);
            float mag = std::sqrt(mA.x * mA.x + mA.y * mA.y);
            azul->setPosicion(std::clamp(azul->getHitbox().x + (mA.x / mag) * 350.f * dt, 25.f, anV - 25.f - azul->getHitbox().ancho),
                std::clamp(azul->getHitbox().y + (mA.y / mag) * 350.f * dt, 25.f, alV - 25.f - azul->getHitbox().alto));
        }
        else { if (azul->getEstado() != ATACANDO) azul->setEstado(QUIETO); }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) mR.y -= 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) mR.y += 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) { mR.x -= 1; mirR = -1.0f; }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) { mR.x += 1; mirR = 1.0f; }
        if (mR.x != 0 || mR.y != 0) {
            rojo->setEstado(CAMINANDO);
            float mag = std::sqrt(mR.x * mR.x + mR.y * mR.y);
            rojo->setPosicion(std::clamp(rojo->getHitbox().x + (mR.x / mag) * 350.f * dt, 25.f, anV - 25.f - rojo->getHitbox().ancho),
                std::clamp(rojo->getHitbox().y + (mR.y / mag) * 350.f * dt, 25.f, alV - 25.f - rojo->getHitbox().alto));
        }
        else { if (rojo->getEstado() != ATACANDO) rojo->setEstado(QUIETO); }

        // ACCIONES
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) arena.comandoDisparoJugador1(mirA, 0.f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::RControl)) arena.comandoDisparoJugador2(mirR, 0.f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) arena.comandoEspecialJugador1();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::P)) arena.comandoEspecialJugador2();

        arena.actualizar(dt); azul->actualizar(dt); rojo->actualizar(dt);

        window.clear();
        window.draw(sprC);

        // RENDERIZADO CON LÓGICA DE ATAQUE
        float cAX = azul->getHitbox().x + azul->getHitbox().ancho / 2;
        float cRX = rojo->getHitbox().x + rojo->getHitbox().ancho / 2;

        // Azul
        if (azul->getEstado() == ATACANDO) sprA.setTexture(atkA);
        else sprA.setTexture(walkA[azul->getFrameActual()]);
        sprA.setOrigin(sprA.getLocalBounds().width / 2, sprA.getLocalBounds().height / 2);
        sprA.setScale(3.0f * mirA, 3.0f);
        sprA.setPosition(cAX, azul->getHitbox().y + azul->getHitbox().alto / 2 + offY);
        if (azul->getVidaActual() < vPA) { sprA.setColor(sf::Color::Red); vPA = azul->getVidaActual(); }
        else sprA.setColor(sf::Color::White);
        window.draw(sprA);

        // Rojo
        if (rojo->getEstado() == ATACANDO) sprR.setTexture(atkR);
        else sprR.setTexture(walkR[rojo->getFrameActual()]);
        sprR.setOrigin(sprR.getLocalBounds().width / 2, sprR.getLocalBounds().height / 2);
        sprR.setScale(3.0f * mirR, 3.0f);
        sprR.setPosition(cRX, rojo->getHitbox().y + rojo->getHitbox().alto / 2 + offY);
        if (rojo->getVidaActual() < vPR) { sprR.setColor(sf::Color::Red); vPR = rojo->getVidaActual(); }
        else sprR.setColor(sf::Color::White);
        window.draw(sprR);

        // Barras de vida
        hpFA.setSize(sf::Vector2f(anchoB * ((float)azul->getVidaActual() / azul->getVidaMaxima()), altoB));
        hpBA.setPosition(cAX - anchoB / 2, azul->getHitbox().y - 20);
        hpFA.setPosition(cAX - anchoB / 2, azul->getHitbox().y - 20);
        window.draw(hpBA); window.draw(hpFA);

        hpFR.setSize(sf::Vector2f(anchoB * ((float)rojo->getVidaActual() / rojo->getVidaMaxima()), altoB));
        hpBR.setPosition(cRX - anchoB / 2, rojo->getHitbox().y - 20);
        hpFR.setPosition(cRX - anchoB / 2, rojo->getHitbox().y - 20);
        window.draw(hpBR); window.draw(hpFR);

        for (auto& p : arena.getProyectiles()) {
            sprB.setPosition(p.getHitbox().x + 10, p.getHitbox().y + 10);
            window.draw(sprB);
        }
        window.display();
    }
    delete azul; delete rojo;
    return 0;
}