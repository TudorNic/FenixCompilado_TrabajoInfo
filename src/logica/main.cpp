#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

// Includes de Combate
#include "Arena.h"
#include "Defensa.h"
#include "Delantero.h"
#include "Lateral.h"
#include "Aficion.h"
#include "Entrenador.h"
#include "ControladorIA.h"

// Includes de Menú
#include "PantallaMenu.h"
#include "PantallaInstrucciones.h"
#include "PantallaRanking.h"
#include "Ranking.h"
#include "PantallaModoJuego.h"

// --- AJUSTE DE LÍMITES (Más cerrados para 800x600) ---
const float L_IZQ = 60.f;
const float L_DER = 740.f;
const float L_SUP = 60.f;
const float L_INF = 540.f;

enum class Estado { MENU, INSTRUCCIONES, RANKING_PANTALLA, MODO_JUEGO, COMBATE };

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Archon Football - Fixed Logic");
    window.setFramerateLimit(60);

    // --- CARGA DE MENÚS (Sergio) ---
    Ranking ranking("../../../assets/texto/ranking.txt");
    ranking.cargar();

    PantallaMenu pMenu(window, "../../../assets/fonts/Bungee-Regular.ttf", "../../../assets/images/menu/fondo_haram.png",
        "../../../assets/images/menu/pelota_futbol.png", "../../../assets/audio/menu/mover_opcion.mp3",
        "../../../assets/audio/menu/confirmar_opcion.mp3", "../../../assets/audio/menu/salir_menu.mp3",
        "../../../assets/audio/menu/musica_haram_fondo_menu.mp3");

    PantallaInstrucciones pInstr(window, "../../../assets/fonts/Bungee-Regular.ttf");
    PantallaRanking pRank(window, ranking, "../../../assets/fonts/Bungee-Regular.ttf");
    PantallaModoJuego pModo(window, "../../../assets/fonts/Bungee-Regular.ttf");

    // --- RECURSOS ---
    sf::Texture texC, texB;
    if (!texC.loadFromFile("campo.png") || !texB.loadFromFile("assets/proyectil/balon_proyectil.png")) return -1;

    Jugador* azul = nullptr, * rojo = nullptr;
    Arena* arena = nullptr;
    ControladorIA* ia = nullptr;
    bool vsIA = true;

    std::vector<sf::Texture> walkA(3), walkR(3), atkA(3), atkR(3);
    sf::Sprite sprA, sprR, sprC(texC), sprB(texB);
    sprB.setOrigin(10, 10);

    sf::RectangleShape hpBA(sf::Vector2f(60, 7)), hpFA(sf::Vector2f(60, 7));
    sf::RectangleShape hpBR(sf::Vector2f(60, 7)), hpFR(sf::Vector2f(60, 7));
    hpBA.setFillColor(sf::Color(50, 50, 50)); hpFA.setFillColor(sf::Color::Green);
    hpBR.setFillColor(sf::Color(50, 50, 50)); hpFR.setFillColor(sf::Color::Green);

    sf::Clock clock;
    Estado estadoActual = Estado::MENU;
    sf::Vector2f uDirA(1.f, 0.f), uDirR(-1.f, 0.f);
    float mirA = 1.0f, mirR = -1.0f;
    bool final = false;

    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();

        switch (estadoActual) {
        case Estado::MENU:
            pMenu.procesarEventos(); pMenu.actualizar(); pMenu.dibujar();
            if (pMenu.estaOpcionConfirmada()) {
                Menu::Opcion op = pMenu.obtenerOpcionConfirmada();
                if (op == Menu::SALIR) window.close();
                else if (op == Menu::INSTRUCCIONES) estadoActual = Estado::INSTRUCCIONES;
                else if (op == Menu::RANKING) estadoActual = Estado::RANKING_PANTALLA;
                else if (op == Menu::JUGAR) estadoActual = Estado::MODO_JUEGO;
                pMenu.reiniciarConfirmacion();
            }
            break;

        case Estado::MODO_JUEGO:
            pModo.procesarEventos(); pModo.actualizar(); pModo.dibujar();
            if (pModo.debeVolverAlMenu()) { pModo.reiniciarVolver(); estadoActual = Estado::MENU; }
            if (pModo.estaOpcionConfirmada()) {
                vsIA = (pModo.obtenerOpcionConfirmada() == PantallaModoJuego::JUGADOR_VS_IA);

                if (azul) delete azul; if (rojo) delete rojo; if (arena) delete arena; if (ia) delete ia;
                azul = new Defensa(1); rojo = new Aficion(); arena = new Arena(azul, rojo);
                if (vsIA) { ia = new ControladorIA(rojo, azul, arena); ia->setDificultad(DificultadIA::DIFICIL); }

                std::string cA = azul->getNombreClase(), cR = rojo->getNombreClase();
                for (int i = 0; i < 3; i++) {
                    walkA[i].loadFromFile("assets/players/blue/" + cA + "/walk/sprite_" + cA + "_blue_walk-" + std::to_string(i + 1) + ".png");
                    walkR[i].loadFromFile("assets/players/red/" + cR + "/walk/sprite_" + cR + "_red_walk-" + std::to_string(i + 1) + ".png");
                    atkA[i].loadFromFile("assets/players/blue/" + cA + "/atack/sprite_" + cA + "_blue_atack-" + std::to_string(i + 1) + ".png");
                    atkR[i].loadFromFile("assets/players/red/" + cR + "/atack/sprite_" + cR + "_red_atack-" + std::to_string(i + 1) + ".png");
                }
                // IMPORTANTE: Fijo el origen al centro del frame (32x32 para sprites de 64x64)
                sprA.setOrigin(32, 32); sprR.setOrigin(32, 32);
                azul->setPosicion(200, 300); rojo->setPosicion(600, 300);

                estadoActual = Estado::COMBATE; final = false; pModo.reiniciarConfirmacion();
            }
            break;

        case Estado::COMBATE:
            sf::Event ev;
            while (window.pollEvent(ev)) { if (ev.type == sf::Event::Closed) window.close(); }

            if (!final) {
                // Control Azul
                sf::Vector2f mA(0.f, 0.f);
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) mA.y -= 1;
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) mA.y += 1;
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) mA.x -= 1;
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) mA.x += 1;

                if (mA.x != 0 || mA.y != 0) {
                    azul->setEstado(CAMINANDO);
                    float mag = std::sqrt(mA.x * mA.x + mA.y * mA.y);
                    uDirA = { mA.x / mag, mA.y / mag };
                    if (mA.x != 0) mirA = (mA.x > 0) ? 1.0f : -1.0f;
                    azul->setPosicion(std::clamp(azul->getHitbox().x + uDirA.x * 350.f * dt, L_IZQ, L_DER - azul->getHitbox().ancho),
                        std::clamp(azul->getHitbox().y + uDirA.y * 350.f * dt, L_SUP, L_INF - azul->getHitbox().alto));
                }
                else if (azul->getEstado() != ATACANDO) azul->setEstado(QUIETO);

                // Control Rojo (IA o P2)
                if (vsIA) ia->actualizar(dt);
                else {
                    sf::Vector2f mR(0.f, 0.f);
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) mR.y -= 1;
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) mR.y += 1;
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) mR.x -= 1;
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) mR.x += 1;
                    if (mR.x != 0 || mR.y != 0) {
                        rojo->setEstado(CAMINANDO);
                        float mag = std::sqrt(mR.x * mR.x + mR.y * mR.y);
                        uDirR = { mR.x / mag, mR.y / mag };
                        if (mR.x != 0) mirR = (mR.x > 0) ? 1.0f : -1.0f;
                        rojo->setPosicion(std::clamp(rojo->getHitbox().x + uDirR.x * 350.f * dt, L_IZQ, L_DER - rojo->getHitbox().ancho),
                            std::clamp(rojo->getHitbox().y + uDirR.y * 350.f * dt, L_SUP, L_INF - rojo->getHitbox().alto));
                    }
                    else if (rojo->getEstado() != ATACANDO) rojo->setEstado(QUIETO);
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::RControl)) arena->comandoDisparoJugador2(uDirR.x, uDirR.y);
                }

                arena->actualizar(dt); azul->actualizar(dt); rojo->actualizar(dt);
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) arena->comandoDisparoJugador1(uDirA.x, uDirA.y);
                if (azul->estaMuerto() || rojo->estaMuerto()) final = true;
            }

            window.clear(); window.draw(sprC);

            // Centrado matemático del Sprite sobre la Hitbox
            // Usamos: Hitbox.x + (Hitbox.ancho / 2)
            sprA.setTexture((azul->getEstado() == ATACANDO) ? atkA[azul->getFrameActual()] : walkA[azul->getFrameActual()]);
            sprA.setScale(3.0f * mirA, 3.0f);
            sprA.setPosition(azul->getHitbox().x + (azul->getHitbox().ancho / 2.0f), azul->getHitbox().y + (azul->getHitbox().alto / 2.0f));
            window.draw(sprA);

            sprR.setTexture((rojo->getEstado() == ATACANDO) ? atkR[rojo->getFrameActual()] : walkR[rojo->getFrameActual()]);
            sprR.setScale(3.0f * mirR, 3.0f);
            sprR.setPosition(rojo->getHitbox().x + (rojo->getHitbox().ancho / 2.0f), rojo->getHitbox().y + (rojo->getHitbox().alto / 2.0f));
            window.draw(sprR);

            // UI y Proyectiles
            hpFA.setSize(sf::Vector2f(60.f * ((float)azul->getVidaActual() / azul->getVidaMaxima()), 7.f));
            hpBA.setPosition(azul->getHitbox().x, azul->getHitbox().y - 15); hpFA.setPosition(azul->getHitbox().x, azul->getHitbox().y - 15);
            window.draw(hpBA); window.draw(hpFA);
            hpFR.setSize(sf::Vector2f(60.f * ((float)rojo->getVidaActual() / rojo->getVidaMaxima()), 7.f));
            hpBR.setPosition(rojo->getHitbox().x, rojo->getHitbox().y - 15); hpFR.setPosition(rojo->getHitbox().x, rojo->getHitbox().y - 15);
            window.draw(hpBR); window.draw(hpFR);

            for (auto& p : arena->getProyectiles()) { sprB.setPosition(p.getHitbox().x + 10, p.getHitbox().y + 10); window.draw(sprB); }
            window.display();

            if (final) { sf::sleep(sf::seconds(1.2f)); estadoActual = Estado::MENU; final = false; }
            break;
        }
    }
    return 0;
}