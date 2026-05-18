#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <ctime>

// Componentes estratégicos del Tablero
#include "tablero.h"
#include "../graficos/tablerografico.h"
#include "IA_Tablero.h"

// Componentes tácticos de la Arena de Combate
#include "Arena.h"
#include "ControladorIA.h"
#include "Jugador.h"
#include "Defensa.h"
#include "Delantero.h"
#include "Lateral.h"
#include "Aficion.h"
#include "Entrenador.h"
#include "Centrocampista.h"

enum class EstadoJuego {
    MENU,
    TABLERO,
    COMBATE
};

// Función auxiliar para instanciar dinámicamente los luchadores en la arena
Jugador* crearLuchadorArena(std::string nombre, int bando) {
    if (nombre == "defensa" || nombre == "Defensa")                 return new Defensa(bando);
    if (nombre == "delantero" || nombre == "Delantero")             return new Delantero(bando);
    if (nombre == "centrocampista" || nombre == "Centrocampista")   return new Centrocampista(bando);
    if (nombre == "lateral" || nombre == "Lateral")                 return new Lateral(bando);
    if (nombre == "entrenador" || nombre == "Entrenador")           return new Entrenador(bando);
    return new Aficion(bando);
}

// Generador de rutas de texturas para el combate 2D
std::string obtenerRutaSpriteArena(std::string nombre, int bando) {
    std::string equipo = (bando == 1) ? "blue" : "red";
    std::string nomLower = nombre;
    std::transform(nomLower.begin(), nomLower.end(), nomLower.begin(), ::tolower);

    if (nomLower == "delantero" && bando == 1) return "assets/players/blue/delantero/atack/sprite_delantero_blue_atack_1.png";
    if (nomLower == "aficion" && bando == 2)   return "assets/players/red/aficion/atack/sprite_aficion_red_atack_1.png";
    if (nomLower == "lateral" || nomLower == "portero") {
        return "assets/players/" + equipo + "/portero/atack/sprite_portero_" + equipo + "_atack-1.png";
    }
    return "assets/players/" + equipo + "/" + nomLower + "/atack/sprite_" + nomLower + "_" + equipo + "_atack-1.png";
}

int main()
{
    srand(static_cast<unsigned int>(time(NULL)));

    sf::RenderWindow window(sf::VideoMode(1000, 1000), "Archon Football - Solución Integrada");
    window.setFramerateLimit(60);

    EstadoJuego estadoActual = EstadoJuego::MENU;
    sf::Clock clock;

    // --- CONFIGURACIÓN DEL MENÚ VISUAL INTEGRADO NATIVO ---
    sf::Font fuenteMenu;
    sf::Texture texFondoMenu;
    sf::Sprite sprFondoMenu;
    bool menuCargadoCorrectamente = true;

    if (!fuenteMenu.loadFromFile("../../../assets/fonts/Bungee-Regular.ttf") ||
        !texFondoMenu.loadFromFile("../../../assets/images/menu/fondo_haram.png")) {
        std::cerr << "Aviso: Error cargando recursos del menú, usando modo texto" << std::endl;
        menuCargadoCorrectamente = false;
    }
    if (menuCargadoCorrectamente) {
        sprFondoMenu.setTexture(texFondoMenu);
        sprFondoMenu.setScale(1000.f / texFondoMenu.getSize().x, 1000.f / texFondoMenu.getSize().y);
    }

    sf::Text txtTitulo("ARCHON FOOTBALL", fuenteMenu, 65);
    txtTitulo.setFillColor(sf::Color::White);
    txtTitulo.setPosition(500.f - txtTitulo.getGlobalBounds().width / 2.f, 150.f);

    sf::Text txtJugar("JUGAR", fuenteMenu, 40);
    sf::Text txtSalir("SALIR", fuenteMenu, 40);
    int opcionSeleccionadaMenu = 0; // Variable corregida para evitar el error C2065

    // --- INICIALIZACIÓN DEL TABLERO TÁCTICO ---
    Tablero logicaTablero;
    TableroGrafico graficosTablero(1.74f);
    graficosTablero.cargarTexturas();

    IATablero cerebroIATablero(2);
    cerebroIATablero.setDificultad(DificultadTablero::DIFICIL);

    int origenX = -1, origenY = -1;
    int combateDestX = -1, combateDestY = -1;

    // Variables de orientación de la Arena
    float dirDisparoX = 1.0f;
    float dirDisparoY = 0.0f;
    std::string claseJugadorActual = "";

    // Mecánicas específicas para la Afición (Segada)
    bool segadaActiva = false;
    float segadaTimer = 0.0f;
    sf::Vector2f dirSegada(0.f, 0.f);

    // Variables dinámicas de la Arena
    Jugador* jugadorAzul = nullptr;
    Jugador* jugadorRojo = nullptr;
    Arena* arenaCombate = nullptr;
    ControladorIA* iaArena = nullptr;

    sf::Texture texLuchadorAzul, texLuchadorRojo;
    sf::Sprite sprLuchadorAzul, sprLuchadorRojo;

    sf::Texture texCampo, texBalon;
    if (!texCampo.loadFromFile("campo.png") || !texBalon.loadFromFile("assets/proyectil/balon_proyectil.png")) {
        std::cerr << "Error cargando assets de la arena" << std::endl;
        return -1;
    }
    sf::Sprite sprCampo(texCampo), sprBalon(texBalon);
    sprBalon.setOrigin(10, 10);

    while (window.isOpen())
    {
        float dt = clock.restart().asSeconds();
        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            switch (estadoActual)
            {
            case EstadoJuego::MENU:
                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::W) {
                        opcionSeleccionadaMenu = 0;
                    }
                    if (event.key.code == sf::Keyboard::Down || event.key.code == sf::Keyboard::S) {
                        opcionSeleccionadaMenu = 1;
                    }
                    if (event.key.code == sf::Keyboard::Enter) {
                        if (opcionSeleccionadaMenu == 0) estadoActual = EstadoJuego::TABLERO;
                        else window.close();
                    }
                }
                break;

            case EstadoJuego::TABLERO:
                if (logicaTablero.getTurnoActual() == 1) {
                    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                        int clicX = event.mouseButton.x / (64 * graficosTablero.getEscala());
                        int clicY = event.mouseButton.y / (64 * graficosTablero.getEscala());

                        if (!logicaTablero.mover_Pieza(clicX, clicY)) {
                            logicaTablero.seleccionar_Pieza(clicX, clicY);
                        }
                    }
                }
                break;

            case EstadoJuego::COMBATE:
                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Space) {
                        std::string claseLower = claseJugadorActual;
                        std::transform(claseLower.begin(), claseLower.end(), claseLower.begin(), ::tolower);

                        // Si es Afición: realiza segada deslizante y NO dispara proyectiles
                        if (claseLower == "aficion") {
                            if (!segadaActiva) {
                                segadaActiva = true;
                                segadaTimer = 0.25f;
                                dirSegada = sf::Vector2f(dirDisparoX, dirDisparoY);
                                sprLuchadorAzul.setRotation(65.f);
                            }
                        }
                        else {
                            // Las demás clases disparan el balón de fútbol
                            arenaCombate->comandoDisparoJugador1(dirDisparoX, dirDisparoY);
                        }
                    }
                    if (event.key.code == sf::Keyboard::E) arenaCombate->comandoEspecialJugador1();
                }
                break;
            }
        }

        window.clear();

        switch (estadoActual)
        {
        case EstadoJuego::MENU:
            if (menuCargadoCorrectamente) {
                window.draw(sprFondoMenu);
            }
            else {
                window.clear(sf::Color(25, 45, 25));
            }

            // Iluminación dinámica de la opción seleccionada (Corregido)
            txtJugar.setFillColor(opcionSeleccionadaMenu == 0 ? sf::Color::Yellow : sf::Color::White);
            txtSalir.setFillColor(opcionSeleccionadaMenu == 1 ? sf::Color::Yellow : sf::Color::White);

            txtJugar.setPosition(500.f - txtJugar.getGlobalBounds().width / 2.f, 450.f);
            txtSalir.setPosition(500.f - txtSalir.getGlobalBounds().width / 2.f, 550.f);

            window.draw(txtTitulo);
            window.draw(txtJugar);
            window.draw(txtSalir);
            break;

        case EstadoJuego::TABLERO:
            if (logicaTablero.getTurnoActual() == 2) {
                sf::sleep(sf::milliseconds(400));
                cerebroIATablero.ejecutarTurno(logicaTablero);
            }

            if (logicaTablero.getCombatePendiente()) {
                origenX = logicaTablero.getCOrigX();
                origenY = logicaTablero.getCOrigY();
                combateDestX = logicaTablero.getCDestX();
                combateDestY = logicaTablero.getCDestY();

                Jugador* atacante = logicaTablero.getCasilla(origenX, origenY);
                Jugador* defensor = logicaTablero.getCasilla(combateDestX, combateDestY);

                if (atacante != nullptr && defensor != nullptr) {
                    claseJugadorActual = atacante->getNombreClase();

                    jugadorAzul = crearLuchadorArena(atacante->getNombreClase(), atacante->getBando());
                    jugadorRojo = crearLuchadorArena(defensor->getNombreClase(), defensor->getBando());

                    arenaCombate = new Arena(jugadorAzul, jugadorRojo);
                    iaArena = new ControladorIA(jugadorRojo, jugadorAzul, arenaCombate);

                    jugadorAzul->setPosicion(150.f, 280.f);
                    jugadorRojo->setPosicion(500.f, 280.f);

                    texLuchadorAzul.loadFromFile(obtenerRutaSpriteArena(atacante->getNombreClase(), atacante->getBando()));
                    texLuchadorRojo.loadFromFile(obtenerRutaSpriteArena(defensor->getNombreClase(), defensor->getBando()));
                    sprLuchadorAzul.setTexture(texLuchadorAzul);
                    sprLuchadorRojo.setTexture(texLuchadorRojo);

                    sprLuchadorAzul.setRotation(0.f);
                    segadaActiva = false;

                    dirDisparoX = 1.0f; dirDisparoY = 0.0f;

                    logicaTablero.resetearCombatePendiente();
                    estadoActual = EstadoJuego::COMBATE;
                }
            }

            graficosTablero.dibujar(window, logicaTablero);
            break;

        case EstadoJuego::COMBATE:
            if (segadaActiva) {
                segadaTimer -= dt;
                float velocidadSegada = 520.f;

                float nuevaX = jugadorAzul->getHitbox().x + (dirSegada.x * velocidadSegada * dt);
                float nuevaY = jugadorAzul->getHitbox().y + (dirSegada.y * velocidadSegada * dt);

                nuevaX = std::clamp(nuevaX, 25.f, 600.f - 25.f - jugadorAzul->getHitbox().ancho);
                nuevaY = std::clamp(nuevaY, 25.f, 560.f - 25.f - jugadorAzul->getHitbox().alto);
                jugadorAzul->setPosicion(nuevaX, nuevaY);

                if (segadaTimer <= 0.0f) {
                    segadaActiva = false;
                    sprLuchadorAzul.setRotation(0.f);
                }
            }
            else {
                sf::Vector2f vA(0.f, 0.f);
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) vA.y -= 1;
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) vA.y += 1;
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) vA.x -= 1;
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) vA.x += 1;

                if (vA.x != 0 || vA.y != 0) {
                    float len = std::sqrt(vA.x * vA.x + vA.y * vA.y);
                    dirDisparoX = vA.x / len;
                    dirDisparoY = vA.y / len;

                    float miVel = jugadorAzul->getVelocidad();
                    jugadorAzul->setPosicion(
                        std::clamp(jugadorAzul->getHitbox().x + (vA.x / len) * miVel * 100.f * dt, 25.f, 600.f - 25.f - jugadorAzul->getHitbox().ancho),
                        std::clamp(jugadorAzul->getHitbox().y + (vA.y / len) * miVel * 100.f * dt, 25.f, 560.f - 25.f - jugadorAzul->getHitbox().alto)
                    );
                }
            }

            iaArena->actualizar(dt);
            arenaCombate->actualizar(dt);
            jugadorAzul->actualizar(dt);
            jugadorRojo->actualizar(dt);

            window.draw(sprCampo);

            // Proporciones visuales equilibradas y nítidas
            sprLuchadorAzul.setScale(3.0f, 3.0f);
            sprLuchadorRojo.setScale(3.0f, 3.0f);
            sprBalon.setScale(0.8f, 0.8f);

            sprLuchadorAzul.setPosition(jugadorAzul->getHitbox().x, jugadorAzul->getHitbox().y);
            sprLuchadorRojo.setPosition(jugadorRojo->getHitbox().x, jugadorRojo->getHitbox().y);
            window.draw(sprLuchadorAzul);
            window.draw(sprLuchadorRojo);

            for (auto& p : arenaCombate->getProyectiles()) {
                if (p.isActivo()) {
                    sprBalon.setPosition(p.getHitbox().x + 10, p.getHitbox().y + 10);
                    window.draw(sprBalon);
                }
            }

            if (arenaCombate->isTerminado() || jugadorAzul->estaMuerto() || jugadorRojo->estaMuerto()) {
                int bandoGanador = (jugadorAzul->estaMuerto()) ? 2 : 1;

                logicaTablero.aplicar_Resultado_Combate(origenX, origenY, combateDestX, combateDestY, bandoGanador);

                delete arenaCombate; delete iaArena; delete jugadorAzul; delete jugadorRojo;
                arenaCombate = nullptr;

                estadoActual = EstadoJuego::TABLERO;
            }
            break;
        }

        window.display();
    }

    return 0;
}