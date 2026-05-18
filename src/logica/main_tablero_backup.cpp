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
#include <ctime>
#include "IA_Tablero.h"
#include "Arena.h"
#include "Defensa.h"
#include "Delantero.h"
#include "Lateral.h"
#include "Aficion.h"
#include "Entrenador.h"
#include "ControladorIA.h"
#include "tablero.h"
#include "../graficos/tablerografico.h"

int main_tablero_antiguo() 
{
    srand(time(NULL));
    /*
    sf::Texture texC, texB;
    if (!texC.loadFromFile("campo.png") || !texB.loadFromFile("assets/proyectil/balon_proyectil.png")) return -1;

    float ancho = (float)texC.getSize().x;
    float alto = (float)texC.getSize().y;
    sf::RenderWindow window(sf::VideoMode((int)ancho, (int)alto), "Archon Football - Dynamic Sprites");

    window.setFramerateLimit(60);

    // --- SELECCIÓN DE JUGADORES ---
    Jugador* azul = new Defensa(1);
    Jugador* rojo = new Entrenador(2);
    Arena arena(azul, rojo);

    //Inicializar IA
    ControladorIA ia(rojo, azul, &arena);
    ia.setDificultad(DificultadIA::DIFICIL);

    // --- CARGA DINÁMICA DE TEXTURAS ---
    std::vector<sf::Texture> walkA(3), walkR(3);
    std::string cA = azul->getNombreClase();
    std::string cR = rojo->getNombreClase();

    for (int i = 0; i < 3; i++) {
        walkA[i].loadFromFile("assets/players/blue/" + cA + "/walk/sprite_" + cA + "_blue_walk-" + std::to_string(i + 1) + ".png");
        walkR[i].loadFromFile("assets/players/red/" + cR + "/walk/sprite_" + cR + "_red_walk-" + std::to_string(i + 1) + ".png");
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
    float L = 25.f; float offY = 40.0f;

    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();
        sf::Event ev;
        while (window.pollEvent(ev)) { if (ev.type == sf::Event::Closed) window.close(); }

        // Movimiento Azul (WASD) HUMANO
        sf::Vector2f vA(0.f, 0.f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) vA.y -= 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) vA.y += 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) { vA.x -= 1; mirA = -1.0f; }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) { vA.x += 1; mirA = 1.0f; }
        if (vA.x != 0 || vA.y != 0) {
            float len = std::sqrt(vA.x * vA.x + vA.y * vA.y);
            float miVelocidad = azul->getVelocidad();
            // Límite X capado a 600.f para cuando reactivéis la arena
            azul->setPosicion(std::clamp(azul->getHitbox().x + (vA.x / len) * miVelocidad * dt, L, 600.f - L - azul->getHitbox().ancho),
                std::clamp(azul->getHitbox().y + (vA.y / len) * miVelocidad * dt, L, alto - L - azul->getHitbox().alto));
        }

        // Acciones (Solo controles del Azul)
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) arena.comandoDisparoJugador1(mirA, 0.f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) arena.comandoEspecialJugador1();

        ia.actualizar(dt);

        arena.actualizar(dt);
        azul->actualizar(dt);
        rojo->actualizar(dt);

        window.clear();
        window.draw(sprC);


        // Renderizado Dinámico
        float cAX = azul->getHitbox().x + azul->getHitbox().ancho / 2;
        float cRX = rojo->getHitbox().x + rojo->getHitbox().ancho / 2;

        sprA.setPosition(cAX, azul->getHitbox().y + azul->getHitbox().alto / 2 + offY);
        sprR.setPosition(cRX, rojo->getHitbox().y + rojo->getHitbox().alto / 2 + offY);
        window.draw(sprA); window.draw(sprR);

        // Barras de Vida
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
    }
    return 0; */


    /******************************
    Apertura del tablero y juego
    *******************************/

    sf::RenderWindow window(sf::VideoMode(1000, 1000), "Prueba");
    window.setFramerateLimit(60);

    Tablero logica;
    logica.Inicializar_Partida(); 
    TableroGrafico graficos(1.74f);

    IATablero cerebroIA(2);
    cerebroIA.setDificultad(DificultadTablero::DIFICIL);

    // Cargamos texturas del tablero
    graficos.cargarTexturas();

    // BUCLE DE JUEGO 
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (logica.getTurnoActual() == 1) {
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {

                        int clicX = event.mouseButton.x / (64 * graficos.getEscala());
                        int clicY = event.mouseButton.y / (64 * graficos.getEscala());

                        if (!logica.mover_Pieza(clicX, clicY)) {
                            logica.seleccionar_Pieza(clicX, clicY);
                        }
                    }
                }
            }
        }
        if (logica.getTurnoActual() == 2) {
            sf::sleep(sf::milliseconds(500));
            cerebroIA.ejecutarTurno(logica);
        }

        window.clear();
        graficos.dibujar(window, logica);
        window.display();
    }
    return 0;
}