


/*
******************************
PRUEBAS IA DE TABLERO
*******************************

#include <iostream>
#include "tablero.h"
#include "Pieza.h"
#include "IA_Tablero.h"

int main() {
    Tablero miTablero;
    miTablero.Inicializar_Partida();
    IATablero cerebroIA(2);

    bool partidaActiva = true;

    while (partidaActiva) {
        miTablero.mostrarTableroprueba();

        // Comprobamos ganador
        int ganador = miTablero.Comprobar_Ganador();
        if (ganador != 0) {
            std::cout << "¡El equipo " << ganador << " ha ganado la partida!" << std::endl;
            break;
        }

        //LÓGICA DE TURNOS
        int turno = miTablero.getTurnoActual(); // Usamos la función que añadimos arriba

        if (turno == 1) {
            int x, y, dx, dy;
            std::cout << "\nTU TURNO (Equipo 1). Introduce origen x e y: ";

            if (!(std::cin >> x >> y)) break;

            if (miTablero.seleccionar_Pieza(x, y)) {
                std::cout << "Introduce destino x e y: ";
                std::cin >> dx >> dy;
                miTablero.mover_Pieza(dx, dy);
            }
        }
        else {
            // TURNO DE TU IA
            std::cout << "\nLA IA (Equipo 2) ESTA PENSANDO...\n";
            cerebroIA.ejecutarTurno(miTablero);
        }
    }

    std::cout << "Presiona ENTER para salir...";
    std::cin.ignore();
    std::cin.get();

    return 0;
}
*/
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include "Arena.h"
#include "Defensa.h"
#include "Delantero.h"
#include "Lateral.h"
#include "Aficion.h"
#include "Entrenador.h"
#include "ControladorIA.h"

int main() {
    sf::Texture texC, texB;
    if (!texC.loadFromFile("campo.png") || !texB.loadFromFile("assets/proyectil/balon_proyectil.png")) return -1;

    float ancho = (float)texC.getSize().x;
    float alto = (float)texC.getSize().y;
    sf::RenderWindow window(sf::VideoMode((int)ancho, (int)alto), "Archon Football - Dynamic Sprites");
    window.setFramerateLimit(60);

    // --- SELECCIÓN DE JUGADORES ---
    Jugador* azul = new Defensa(1);
    Jugador* rojo = new Entrenador();
    Arena arena(azul, rojo);

    //Inicializar IA
    ControladorIA ia(rojo, azul, &arena);
    ia.setDificultad(DificultadIA::DIFICIL);

    // --- CARGA DINÁMICA DE TEXTURAS ---
    std::vector<sf::Texture> walkA(3), walkR(3);
    std::vector<sf::Texture> atkA(3), atkR(3);
    std::string cA = azul->getNombreClase();
    std::string cR = rojo->getNombreClase();
   
    for (int i = 0; i < 3; i++) {
        // Texturas de Caminar 
        walkA[i].loadFromFile("assets/players/blue/" + cA + "/walk/sprite_" + cA + "_blue_walk-" + std::to_string(i + 1) + ".png");
        walkR[i].loadFromFile("assets/players/red/" + cR + "/walk/sprite_" + cR + "_red_walk-" + std::to_string(i + 1) + ".png");

        // Texturas de Ataque 
        atkA[i].loadFromFile("assets/players/blue/" + cA + "/atack/sprite_" + cA + "_blue_atack-" + std::to_string(i + 1) + ".png");
        atkR[i].loadFromFile("assets/players/red/" + cR + "/atack/sprite_" + cR + "_red_atack-" + std::to_string(i + 1) + ".png");
    }

    sf::Sprite sprA(walkA[0]), sprR(walkR[0]), sprC(texC), sprB(texB);
    sprA.setOrigin(32, 32); sprR.setOrigin(32, 32); sprB.setOrigin(10, 10);
    sprA.setScale(3.0f, 3.0f); sprR.setScale(3.0f, 3.0f);

    // UI
    float anchoB = 60.0f; float altoB = 7.0f;
    sf::RectangleShape hpBA(sf::Vector2f(anchoB, altoB)), hpFA(sf::Vector2f(anchoB, altoB));
    sf::RectangleShape hpBR(sf::Vector2f(anchoB, altoB)), hpFR(sf::Vector2f(anchoB, altoB));
    hpBA.setFillColor(sf::Color(50, 50, 50)); hpFA.setFillColor(sf::Color::Green);
    hpBR.setFillColor(sf::Color(50, 50, 50)); hpFR.setFillColor(sf::Color::Green);

    azul->setPosicion(ancho * 0.2f, alto * 0.5f);
    rojo->setPosicion(ancho * 0.8f, alto * 0.5f);
    sf::Clock clock;
    float mirA = 1.0f, mirR = -1.0f;
    sf::Vector2f ultimaDirA(1.f, 0.f);
    float offY = 40.0f;
    bool partidaFinalizada = false;

    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();
        sf::Event ev;
        while (window.pollEvent(ev)) { if (ev.type == sf::Event::Closed) window.close(); }
        if (!partidaFinalizada) {
            // --- MOVIMIENTO AZUL ---
            sf::Vector2f mA(0.f, 0.f);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) mA.y -= 1;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) mA.y += 1;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) mA.x -= 1;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) mA.x += 1;

            if (mA.x != 0 || mA.y != 0) {
                azul->setEstado(CAMINANDO);
                float mag = std::sqrt(mA.x * mA.x + mA.y * mA.y);
                sf::Vector2f dirNormalizada = { mA.x / mag, mA.y / mag };

                // Actualizamos la dirección de apuntado y el espejo visual
                ultimaDirA = dirNormalizada;
                if (mA.x != 0) mirA = (mA.x > 0) ? 1.0f : -1.0f;

                azul->setPosicion(std::clamp(azul->getHitbox().x + dirNormalizada.x * 350.f * dt, 25.f, ancho - 25.f - azul->getHitbox().ancho),
                    std::clamp(azul->getHitbox().y + dirNormalizada.y * 350.f * dt, 25.f, alto - 25.f - azul->getHitbox().alto));
            }
            else { if (azul->getEstado() != ATACANDO) azul->setEstado(QUIETO); }

            // --- MOVIMIENTO ROJO ---
            ia.actualizar(dt);

            arena.actualizar(dt);
            azul->actualizar(dt);
            rojo->actualizar(dt);
            /*MOVIMIENTO ROJO de rama de integracion_logica_graficos (desactivado para pruebas de IA)
            sf::Vector2f mR(0.f, 0.f);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) mR.y -= 1;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) mR.y += 1;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) mR.x -= 1;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) mR.x += 1;

            if (mR.x != 0 || mR.y != 0) {
                rojo->setEstado(CAMINANDO);
                float mag = std::sqrt(mR.x * mR.x + mR.y * mR.y);
                sf::Vector2f dirNormalizada = { mR.x / mag, mR.y / mag };

                ultimaDirR = dirNormalizada;
                if (mR.x != 0) mirR = (mR.x > 0) ? 1.0f : -1.0f;

                rojo->setPosicion(std::clamp(rojo->getHitbox().x + dirNormalizada.x * 350.f * dt, 25.f, anV - 25.f - rojo->getHitbox().ancho),
                    std::clamp(rojo->getHitbox().y + dirNormalizada.y * 350.f * dt, 25.f, alV - 25.f - rojo->getHitbox().alto));
            }
            else { if (rojo->getEstado() != ATACANDO) rojo->setEstado(QUIETO); }

            // --- ACCIONES (DISPARO MULTIDIRECCIONAL) ---
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
                arena.comandoDisparoJugador1(ultimaDirA.x, ultimaDirA.y);

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))
                arena.comandoDisparoJugador2(ultimaDirR.x, ultimaDirR.y);

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) arena.comandoEspecialJugador1();
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::P)) arena.comandoEspecialJugador2();

            arena.actualizar(dt); azul->actualizar(dt); rojo->actualizar(dt);

            // COMPROBACIÓN DE MUERTE
            if (azul->estaMuerto() || rojo->estaMuerto()) {
                partidaFinalizada = true;
            }*/
        }

        // Acciones (Solo controles del Azul)
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) arena.comandoDisparoJugador1(ultimaDirA.x, ultimaDirA.y);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) arena.comandoEspecialJugador1();

        if (azul->estaMuerto() || rojo->estaMuerto()) partidaFinalizada = true;
    
        window.clear();
        window.draw(sprC);


        // Renderizado Dinámico
        float cAX = azul->getHitbox().x + azul->getHitbox().ancho / 2;
        float cRX = rojo->getHitbox().x + rojo->getHitbox().ancho / 2;

        // Azul: Render
        if (azul->getEstado() == ATACANDO) {
            sprA.setTexture(atkA[azul->getFrameActual()]);
        }
        else {
            sprA.setTexture(walkA[azul->getFrameActual()]);
        }

        sprA.setRotation(azul->estaMuerto() ? 90.f : 0.f);
        sprA.setColor(azul->estaMuerto() ? sf::Color(255, 255, 255, 120) : sf::Color::White);
        sprA.setOrigin(sprA.getLocalBounds().width / 2, sprA.getLocalBounds().height / 2);
        sprA.setScale(3.0f * mirA, 3.0f);
        sprA.setPosition(cAX, azul->getHitbox().y + azul->getHitbox().alto / 2 + offY);
        window.draw(sprA);

        // Rojo: Render
        if (rojo->getEstado() == ATACANDO) {
            sprR.setTexture(atkR[rojo->getFrameActual()]); // <--- AHORA CARGA EL FRAME 1, 2 o 3
        }
        else {
            sprR.setTexture(walkR[rojo->getFrameActual()]);
        }

        sprR.setRotation(rojo->estaMuerto() ? -90.f : 0.f);
        sprR.setColor(rojo->estaMuerto() ? sf::Color(255, 255, 255, 120) : sf::Color::White);
        sprR.setOrigin(sprR.getLocalBounds().width / 2, sprR.getLocalBounds().height / 2);
        sprR.setScale(3.0f * mirR, 3.0f);
        sprR.setPosition(cRX, rojo->getHitbox().y + rojo->getHitbox().alto / 2 + offY);
        window.draw(sprR);

        // Barras de Vida (Color dinámico para Entrenador)
        hpFA.setSize(sf::Vector2f(anchoB * ((float)azul->getVidaActual() / azul->getVidaMaxima()), altoB));
        hpFA.setFillColor(azul->getNombreClase() == "entrenador" ? sf::Color::Cyan : sf::Color::Green);
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

        if (partidaFinalizada) {
            sf::sleep(sf::seconds(2.0f));
            window.close();
        }
    }
    return 0;
}