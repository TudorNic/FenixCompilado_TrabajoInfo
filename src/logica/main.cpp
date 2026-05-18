#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp> 
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
    INSTRUCCIONES,
    RANKING_PANTALLA,
    MODO_JUEGO,
    TABLERO,
    COMBATE
};

// Función auxiliar robusta frente a acentos para instanciar luchadores
Jugador* crearLuchadorArena(std::string nombre, int bando) {
    std::string n = nombre;
    std::transform(n.begin(), n.end(), n.begin(), ::tolower);

    if (n.find("defen") != std::string::npos)      return new Defensa(bando);
    if (n.find("delan") != std::string::npos)      return new Delantero(bando);
    if (n.find("centro") != std::string::npos)     return new Centrocampista(bando);
    if (n.find("later") != std::string::npos)      return new Lateral(bando);
    if (n.find("entren") != std::string::npos)     return new Entrenador(bando);
    return new Aficion(bando);
}

// Generador automatizado y blindado contra acentos para las rutas de sprites
std::string obtenerRutaSpriteArena(std::string nombre, int bando) {
    std::string equipo = (bando == 1) ? "blue" : "red";
    std::string nomLower = nombre;
    std::transform(nomLower.begin(), nomLower.end(), nomLower.begin(), ::tolower);

    if (nomLower.find("delan") != std::string::npos && bando == 1) return "assets/players/blue/delantero/atack/sprite_delantero_blue_atack_1.png";
    if (nomLower.find("afic") != std::string::npos && bando == 2)   return "assets/players/red/aficion/atack/sprite_aficion_red_atack_1.png";

    if (nomLower.find("later") != std::string::npos || nomLower.find("porte") != std::string::npos) {
        return "assets/players/" + equipo + "/portero/atack/sprite_portero_" + equipo + "_atack-1.png";
    }

    std::string carpeta = nomLower;
    if (carpeta.find("afic") != std::string::npos) carpeta = "aficion";
    if (carpeta.find("defen") != std::string::npos) carpeta = "defensa";
    if (carpeta.find("centro") != std::string::npos) carpeta = "centrocampista";
    if (carpeta.find("entren") != std::string::npos) carpeta = "entrenador";

    return "assets/players/" + equipo + "/" + carpeta + "/atack/sprite_" + carpeta + "_" + equipo + "_atack-1.png";
}

int main()
{
    srand(static_cast<unsigned int>(time(NULL)));

    sf::RenderWindow window(sf::VideoMode(1000, 1000), "Archon Football - Solución Integrada");
    window.setFramerateLimit(60);

    EstadoJuego estadoActual = EstadoJuego::MENU;
    sf::Clock clock;

    // --- AUDIO NATIVO ---
    sf::Music musicaMenu;
    sf::SoundBuffer bufMover, bufConfirmar;
    sf::Sound sndMover, sndConfirmar;

    if (musicaMenu.openFromFile("../../../assets/audio/menu/musica_haram_fondo_menu.mp3")) {
        musicaMenu.setLoop(true);
        musicaMenu.setVolume(35.f);
        musicaMenu.play();
    }
    if (bufMover.loadFromFile("../../../assets/audio/menu/mover_opcion.mp3"))       sndMover.setBuffer(bufMover);
    if (bufConfirmar.loadFromFile("../../../assets/audio/menu/confirmar_opcion.mp3")) sndConfirmar.setBuffer(bufConfirmar);

    // --- FUENTES Y FONDOS ---
    sf::Font fuenteMenu;
    sf::Texture texFondoMenu;
    sf::Sprite sprFondoMenu;
    bool recursosCargados = true;

    if (!fuenteMenu.loadFromFile("../../../assets/fonts/Bungee-Regular.ttf") ||
        !texFondoMenu.loadFromFile("../../../assets/images/menu/fondo_haram.png")) {
        recursosCargados = false;
    }
    if (recursosCargados) {
        sprFondoMenu.setTexture(texFondoMenu);
        sprFondoMenu.setScale(1000.f / texFondoMenu.getSize().x, 1000.f / texFondoMenu.getSize().y);
    }

    sf::Text txtTitulo("ARCHON FOOTBALL", fuenteMenu, 60);
    txtTitulo.setPosition(500.f - txtTitulo.getGlobalBounds().width / 2.f, 100.f);

    sf::Text opcionesMenu[4];
    std::string nombresOpciones[4] = { "JUGAR", "RANKING", "INSTRUCCIONES", "SALIR" };
    int idxMenuPrincipal = 0;

    sf::Text txtSubTitulo("SELECCIONA MODO DE JUEGO", fuenteMenu, 40);
    sf::Text opcionesModo[2];
    std::string nombresModos[2] = { "JUGADOR VS IA", "JUGADOR VS JUGADOR" };
    int idxModoJuego = 0;
    bool esModoPvP = false;

    // --- TABLERO TÁCTICO ---
    Tablero logicaTablero;
    TableroGrafico graficosTablero(1.74f);
    graficosTablero.cargarTexturas();

    IATablero cerebroIATablero(2);
    cerebroIATablero.setDificultad(DificultadTablero::DIFICIL);

    int origenX = -1, origenY = -1;
    int combateDestX = -1, combateDestY = -1;

    float dirDisparoX1 = 1.0f;  float dirDisparoY1 = 0.0f;
    float dirDisparoX2 = -1.0f; float dirDisparoY2 = 0.0f;
    std::string claseJugadorActual = "";

    // Mecánica de Segada (Afición)
    bool segadaActiva = false; float segadaTimer = 0.0f; sf::Vector2f dirSegada(0.f, 0.f);

    // Arena de combate
    Jugador* jugadorAzul = nullptr; Jugador* jugadorRojo = nullptr;
    Arena* arenaCombate = nullptr; ControladorIA* iaArena = nullptr;

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

            if (estadoActual == EstadoJuego::MENU) {
                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::W) {
                        idxMenuPrincipal = (idxMenuPrincipal - 1 + 4) % 4; sndMover.play();
                    }
                    if (event.key.code == sf::Keyboard::Down || event.key.code == sf::Keyboard::S) {
                        idxMenuPrincipal = (idxMenuPrincipal + 1) % 4; sndMover.play();
                    }
                    if (event.key.code == sf::Keyboard::Enter) {
                        sndConfirmar.play();
                        if (idxMenuPrincipal == 0)      estadoActual = EstadoJuego::MODO_JUEGO;
                        else if (idxMenuPrincipal == 1) estadoActual = EstadoJuego::RANKING_PANTALLA;
                        else if (idxMenuPrincipal == 2) estadoActual = EstadoJuego::INSTRUCCIONES;
                        else window.close();
                    }
                }
            }
            else if (estadoActual == EstadoJuego::MODO_JUEGO) {
                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::W) {
                        idxModoJuego = 0; sndMover.play();
                    }
                    if (event.key.code == sf::Keyboard::Down || event.key.code == sf::Keyboard::S) {
                        idxModoJuego = 1; sndMover.play();
                    }
                    if (event.key.code == sf::Keyboard::Escape) {
                        estadoActual = EstadoJuego::MENU;
                    }
                    if (event.key.code == sf::Keyboard::Enter) {
                        sndConfirmar.play();
                        esModoPvP = (idxModoJuego == 1);
                        musicaMenu.stop();
                        estadoActual = EstadoJuego::TABLERO;
                    }
                }
            }
            else if (estadoActual == EstadoJuego::INSTRUCCIONES || estadoActual == EstadoJuego::RANKING_PANTALLA) {
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                    estadoActual = EstadoJuego::MENU;
                }
            }
            else {
                switch (estadoActual)
                {
                case EstadoJuego::TABLERO:
                    if (logicaTablero.getTurnoActual() == 1 || (logicaTablero.getTurnoActual() == 2 && esModoPvP)) {
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

                            if (claseLower.find("afic") != std::string::npos) {
                                if (!segadaActiva) {
                                    segadaActiva = true; segadaTimer = 0.25f;
                                    dirSegada = sf::Vector2f(dirDisparoX1, dirDisparoY1);
                                    sprLuchadorAzul.setRotation(65.f);
                                }
                            }
                            else {
                                arenaCombate->comandoDisparoJugador1(dirDisparoX1, dirDisparoY1);
                            }
                        }
                        if (event.key.code == sf::Keyboard::E) arenaCombate->comandoEspecialJugador1();

                        if (esModoPvP && event.key.code == sf::Keyboard::RControl) {
                            if (jugadorRojo->getNombreClase().find("Afic") == std::string::npos && jugadorRojo->getNombreClase().find("afic") == std::string::npos) {
                                arenaCombate->comandoDisparoJugador2(dirDisparoX2, dirDisparoY2);
                            }
                        }
                        if (esModoPvP && event.key.code == sf::Keyboard::RShift) {
                            arenaCombate->comandoEspecialJugador2();
                        }
                    }
                    break;
                }
            }
        }

        switch (estadoActual)
        {
        case EstadoJuego::MENU:
            if (recursosCargados) window.draw(sprFondoMenu);
            for (int i = 0; i < 4; i++) {
                opcionesMenu[i].setFont(fuenteMenu);
                opcionesMenu[i].setString(nombresOpciones[i]);
                opcionesMenu[i].setCharacterSize(40);
                opcionesMenu[i].setFillColor(idxMenuPrincipal == i ? sf::Color::Yellow : sf::Color::White);
                opcionesMenu[i].setPosition(500.f - opcionesMenu[i].getGlobalBounds().width / 2.f, 350.f + i * 90.f);
                window.draw(opcionesMenu[i]);
            }
            window.draw(txtTitulo);
            break;

        case EstadoJuego::MODO_JUEGO:
            if (recursosCargados) window.draw(sprFondoMenu);
            txtSubTitulo.setPosition(500.f - txtSubTitulo.getGlobalBounds().width / 2.f, 250.f);
            window.draw(txtSubTitulo);
            for (int i = 0; i < 2; i++) {
                opcionesModo[i].setFont(fuenteMenu);
                opcionesModo[i].setString(nombresModos[i]);
                opcionesModo[i].setCharacterSize(38);
                opcionesModo[i].setFillColor(idxModoJuego == i ? sf::Color::Yellow : sf::Color::White);
                opcionesModo[i].setPosition(500.f - opcionesModo[i].getGlobalBounds().width / 2.f, 480.f + i * 100.f);
                window.draw(opcionesModo[i]);
            }
            break;

        case EstadoJuego::INSTRUCCIONES:
            window.clear(sf::Color(15, 30, 15));
            {
                sf::Text tx("COMO JUGAR", fuenteMenu, 50);
                tx.setPosition(500.f - tx.getGlobalBounds().width / 2.f, 100.f); window.draw(tx);
                sf::Text body("P1 (AZUL): Mover con WASD\nAtacar/Segada con ESPACIO\nEspecial con tecla E\n\nP2 (ROJO en PvP): Mover con FLECHAS\nAtacar con CONTROL DERECHO\nEspecial con SHIFT DERECHO\n\nPresiona ESCAPE para volver.", fuenteMenu, 25);
                body.setPosition(150.f, 300.f); window.draw(body);
            }
            break;

        case EstadoJuego::RANKING_PANTALLA:
            window.clear(sf::Color(25, 25, 35));
            {
                sf::Text tx("RANKING DE JUGADORES", fuenteMenu, 50);
                tx.setPosition(500.f - tx.getGlobalBounds().width / 2.f, 100.f); window.draw(tx);
                sf::Text body("1. JORGE - 1500 PTS\n2. TUDOR - 1200 PTS\n3. GABRI - 950 PTS\n4. HUGO - 800 PTS\n\nPresiona ESCAPE para salir.", fuenteMenu, 30);
                body.setPosition(300.f, 350.f); window.draw(body);
            }
            break;

        case EstadoJuego::TABLERO:
            if (logicaTablero.getTurnoActual() == 2 && !esModoPvP) {
                sf::sleep(sf::milliseconds(400)); cerebroIATablero.ejecutarTurno(logicaTablero);
            }

            if (logicaTablero.getCombatePendiente()) {
                origenX = logicaTablero.getCOrigX(); origenY = logicaTablero.getCOrigY();
                combateDestX = logicaTablero.getCDestX(); combateDestY = logicaTablero.getCDestY();

                Jugador* atacante = logicaTablero.getCasilla(origenX, origenY);
                Jugador* defensor = logicaTablero.getCasilla(combateDestX, combateDestY);

                if (atacante != nullptr && defensor != nullptr) {
                    claseJugadorActual = atacante->getNombreClase();

                    jugadorAzul = crearLuchadorArena(atacante->getNombreClase(), atacante->getBando());
                    jugadorRojo = crearLuchadorArena(defensor->getNombreClase(), defensor->getBando());

                    arenaCombate = new Arena(jugadorAzul, jugadorRojo);
                    if (!esModoPvP) iaArena = new ControladorIA(jugadorRojo, jugadorAzul, arenaCombate);

                    jugadorAzul->setPosicion(150.f, 280.f);
                    jugadorRojo->setPosicion(500.f, 280.f);

                    texLuchadorAzul.loadFromFile(obtenerRutaSpriteArena(atacante->getNombreClase(), atacante->getBando()));
                    texLuchadorRojo.loadFromFile(obtenerRutaSpriteArena(defensor->getNombreClase(), defensor->getBando()));
                    sprLuchadorAzul.setTexture(texLuchadorAzul);
                    sprLuchadorRojo.setTexture(texLuchadorRojo);

                    sprLuchadorAzul.setRotation(0.f); sprLuchadorRojo.setRotation(0.f);
                    segadaActiva = false;

                    dirDisparoX1 = 1.0f;  dirDisparoY1 = 0.0f;
                    dirDisparoX2 = -1.0f; dirDisparoY2 = 0.0f;

                    logicaTablero.resetearCombatePendiente();
                    estadoActual = EstadoJuego::COMBATE;
                }
            }

            logicaTablero.Comprobar_Ganador();
            graficosTablero.dibujar(window, logicaTablero);
            break;

        case EstadoJuego::COMBATE:
            // --- ACTUALIZACIÓN JUGADOR 1 (LÍMITES TOTALMENTE COMPRIMIDOS AL ESTADIO AZUL) ---
            if (segadaActiva) {
                segadaTimer -= dt;
                float velocidadSegada = 520.f;
                float nuevaX = jugadorAzul->getHitbox().x + (dirSegada.x * velocidadSegada * dt);
                float nuevaY = jugadorAzul->getHitbox().y + (dirSegada.y * velocidadSegada * dt);

                nuevaX = std::clamp(nuevaX, 25.f, 600.f - 25.f - jugadorAzul->getHitbox().ancho);
                nuevaY = std::clamp(nuevaY, 25.f, 560.f - 25.f - jugadorAzul->getHitbox().alto);
                jugadorAzul->setPosicion(nuevaX, nuevaY);

                if (segadaTimer <= 0.0f) {
                    segadaActiva = false; sprLuchadorAzul.setRotation(0.f);
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
                    dirDisparoX1 = vA.x / len; dirDisparoY1 = vA.y / len;

                    float miVel = jugadorAzul->getVelocidad();
                    jugadorAzul->setPosicion(
                        std::clamp(jugadorAzul->getHitbox().x + (vA.x / len) * miVel * 100.f * dt, 25.f, 600.f - 25.f - jugadorAzul->getHitbox().ancho),
                        std::clamp(jugadorAzul->getHitbox().y + (vA.y / len) * miVel * 100.f * dt, 25.f, 560.f - 25.f - jugadorAzul->getHitbox().alto)
                    );
                }
            }

            // --- CORRECCIÓN INTEGRAL: LÍMITES CERRADOS PARA EL JUGADOR 2 / IA EN EL CAMPO AZUL ---
            if (esModoPvP) {
                sf::Vector2f vR(0.f, 0.f);
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))    vR.y -= 1;
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))  vR.y += 1;
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))  vR.x -= 1;
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) vR.x += 1;

                if (vR.x != 0 || vR.y != 0) {
                    float len = std::sqrt(vR.x * vR.x + vR.y * vR.y);
                    dirDisparoX2 = vR.x / len; dirDisparoY2 = vR.y / len;

                    float velRed = jugadorRojo->getVelocidad();
                    jugadorRojo->setPosicion(
                        std::clamp(jugadorRojo->getHitbox().x + (vR.x / len) * velRed * 100.f * dt, 25.f, 600.f - 25.f - jugadorRojo->getHitbox().ancho),
                        std::clamp(jugadorRojo->getHitbox().y + (vR.y / len) * velRed * 100.f * dt, 25.f, 560.f - 25.f - jugadorRojo->getHitbox().alto)
                    );
                }
            }
            else {
                iaArena->actualizar(dt);
            }

            arenaCombate->actualizar(dt);
            jugadorAzul->actualizar(dt);
            jugadorRojo->actualizar(dt);

            window.draw(sprCampo);

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

            // ========================================================
            // --- CÁLCULO ARREGLADO: BARRAS FLOTANTES CENTRADAS AL 100% ---
            // ========================================================
            float barWidth = 50.f;

            // 1. Barra Flotante P1 (Se multiplica por 3.0f vuestro ancho para clavar el centro visual)
            float pctAzul = static_cast<float>(jugadorAzul->getVidaActual()) / static_cast<float>(jugadorAzul->getVidaMaxima());
            sf::RectangleShape fBarraAzul(sf::Vector2f(barWidth, 5.f));
            fBarraAzul.setPosition(
                jugadorAzul->getHitbox().x + ((jugadorAzul->getHitbox().ancho * 3.0f) / 2.f) - (barWidth / 2.f),
                jugadorAzul->getHitbox().y - 15.f
            );
            fBarraAzul.setFillColor(sf::Color(40, 40, 40));
            sf::RectangleShape bVidaAzul(sf::Vector2f(barWidth * pctAzul, 5.f));
            bVidaAzul.setPosition(fBarraAzul.getPosition());
            bVidaAzul.setFillColor(sf::Color::Green); // P1 siempre es verde (tu aliado/tú)

            window.draw(fBarraAzul); window.draw(bVidaAzul);

            // 2. Barra Flotante P2 / IA (Centrada multiplicando vuestro ancho por 3.0f)
            float pctRojo = static_cast<float>(jugadorRojo->getVidaActual()) / static_cast<float>(jugadorRojo->getVidaMaxima());
            sf::RectangleShape fBarraRojo(sf::Vector2f(barWidth, 5.f));
            fBarraRojo.setPosition(
                jugadorRojo->getHitbox().x + ((jugadorRojo->getHitbox().ancho * 3.0f) / 2.f) - (barWidth / 2.f),
                jugadorRojo->getHitbox().y - 15.f
            );
            fBarraRojo.setFillColor(sf::Color(40, 40, 40));
            sf::RectangleShape bVidaRojo(sf::Vector2f(barWidth * pctRojo, 5.f));
            bVidaRojo.setPosition(fBarraRojo.getPosition());
            bVidaRojo.setFillColor(sf::Color::Red); // Enemigo/P2 siempre es rojo

            window.draw(fBarraRojo); window.draw(bVidaRojo);

            if (arenaCombate->isTerminado() || jugadorAzul->estaMuerto() || jugadorRojo->estaMuerto()) {
                int bandoGanador = (jugadorAzul->estaMuerto()) ? 2 : 1;

                logicaTablero.aplicar_Resultado_Combate(origenX, origenY, combateDestX, combateDestY, bandoGanador);

                if (!esModoPvP) delete iaArena;
                delete arenaCombate; delete jugadorAzul; delete jugadorRojo;
                arenaCombate = nullptr;

                estadoActual = EstadoJuego::TABLERO;
            }
            break;
        }

        window.display();
    }

    return 0;
}