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

//Componentes correspondientes a la pantalla del menu
#include "PantallaMenu.h"
#include "PantallaModoJuego.h"
#include "PantallaInstrucciones.h"
#include "PantallaRanking.h"
#include "Ranking.h"
#include "PantallaDificultadIA.h"

//Sonido
#include "SonidoJuego.h"

enum class EstadoJuego {
    MENU,
    INSTRUCCIONES,
    RANKING_PANTALLA,
    MODO_JUEGO,
    DIFICULTAD_IA,
    TABLERO,
    COMBATE,
    FIN_PARTIDA
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

        //---FIN PARTIDA---
    int equipoGanador = 0;

    sf::Font fuenteFin;
    fuenteFin.loadFromFile("../../../assets/fonts/Bungee-Regular.ttf");

    sf::Text textoFin;
    textoFin.setFont(fuenteFin);
    textoFin.setCharacterSize(36);
    textoFin.setFillColor(sf::Color::White);
    textoFin.setPosition(180.f, 380.f);

    sf::Text textoVolverMenu;
    textoVolverMenu.setFont(fuenteFin);
    textoVolverMenu.setCharacterSize(22);
    textoVolverMenu.setFillColor(sf::Color::Yellow);
    textoVolverMenu.setString("Pulsa ENTER para volver al menu");
    textoVolverMenu.setPosition(220.f, 480.f);

 
        // --- RANKING ---

    Ranking ranking("../../../assets/texto/ranking.txt");
    ranking.cargar();

    
    // --- PANTALLAS DE MENU ---
 
    PantallaMenu pantallaMenu(
        window,
        "../../../assets/fonts/Bungee-Regular.ttf",
        "../../../assets/images/menu/fondo_haram.png",
        "../../../assets/images/menu/pelota_futbol.png",
        "../../../assets/audio/menu/mover_opcion.mp3",
        "../../../assets/audio/menu/confirmar_opcion.mp3",
        "../../../assets/audio/menu/salir_menu.mp3",
        "../../../assets/audio/menu/musica_haram_fondo_menu.mp3"
    );

    PantallaModoJuego pantallaModoJuego(
        window,
        "../../../assets/fonts/Bungee-Regular.ttf",
		"../../../assets/images/menu/fondo_haram.png",
        "../../../assets/audio/menu/mover_opcion.mp3"
    );

    PantallaDificultadIA pantallaDificultadIA(
        window,
        "../../../assets/fonts/Bungee-Regular.ttf",
        "../../../assets/images/menu/fondo_haram.png",
        "../../../assets/audio/menu/mover_opcion.mp3"
    );

    PantallaInstrucciones pantallaInstrucciones(
        window,
        "../../../assets/fonts/Bungee-Regular.ttf"
    );

    PantallaRanking pantallaRanking(
        window,
        ranking,
        "../../../assets/fonts/Bungee-Regular.ttf"
    );
    //Sonido juego
    SonidoJuego sonidoJuego(
        "../../../assets/audio/tablero/Sonido_ambiente_estadio.mp3",
        "../../../assets/audio/tablero/Silbato_arbitro.mp3"
    );

    // --- TABLERO TÁCTICO ---
    Tablero logicaTablero;
    TableroGrafico graficosTablero(1.74f);
    graficosTablero.cargarTexturas();

    IATablero cerebroIATablero(2);
    cerebroIATablero.setDificultad(DificultadTablero::DIFICIL);

	bool esModoPvP = false;

    int origenX = -1, origenY = -1;
    int combateDestX = -1, combateDestY = -1;

    float dirDisparoX1 = 1.0f;  float dirDisparoY1 = 0.0f;
    float dirDisparoX2 = -1.0f; float dirDisparoY2 = 0.0f;
    std::string claseJugadorActual = "";

    // Mecánica de Segada (Afición)
    bool segadaActiva = false; float segadaTimer = 0.0f; sf::Vector2f dirSegada(0.f, 0.f);

    // Tiempo espera fin cada combate
    bool esperandoFinCombate = false;
    float timerFinCombate = 0.0f;
    const float TIEMPO_ESPERA_FIN_COMBATE = 4.0f;
    int bandoGanadorCombate = 0;


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

        // --- PANTALLAS DEL MENU CON LAS CLASES ---

        if (estadoActual == EstadoJuego::MENU)
        {
            pantallaMenu.procesarEventos();
            pantallaMenu.actualizar();
            pantallaMenu.dibujar();

            if (pantallaMenu.estaOpcionConfirmada())
            {
                Menu::Opcion opcion = pantallaMenu.obtenerOpcionConfirmada();

                if (opcion == Menu::JUGAR)
                {
                    estadoActual = EstadoJuego::MODO_JUEGO;
                }
                else if (opcion == Menu::RANKING)
                {
                    estadoActual = EstadoJuego::RANKING_PANTALLA;
                }
                else if (opcion == Menu::INSTRUCCIONES)
                {
                    estadoActual = EstadoJuego::INSTRUCCIONES;
                }
                else if (opcion == Menu::SALIR)
                {
                    window.close();
                }

                pantallaMenu.reiniciarConfirmacion();
            }

            continue;
        }

        if (estadoActual == EstadoJuego::MODO_JUEGO)
        {
            pantallaModoJuego.procesarEventos();
            pantallaModoJuego.actualizar();
            pantallaModoJuego.dibujar();

            if (pantallaModoJuego.debeVolverAlMenu())
            {
                pantallaModoJuego.reiniciarVolver();
                estadoActual = EstadoJuego::MENU;

                ////AÑADIDO PARA AL FINALIZAR PARTIDA REINICIAR
                int equipoGanador = 0;

                sf::Font fuenteFin;
                fuenteFin.loadFromFile("../../../assets/fonts/Bungee-Regular.ttf");

                sf::Text textoFin;
                textoFin.setFont(fuenteFin);
                textoFin.setCharacterSize(36);
                textoFin.setFillColor(sf::Color::White);
                textoFin.setPosition(180.f, 380.f);

                sf::Text textoVolverMenu;
                textoVolverMenu.setFont(fuenteFin);
                textoVolverMenu.setCharacterSize(22);
                textoVolverMenu.setFillColor(sf::Color::Yellow);
                textoVolverMenu.setString("Pulsa ENTER para volver al menu");
                textoVolverMenu.setPosition(220.f, 480.f);
            }

            if (pantallaModoJuego.estaOpcionConfirmada())
            {
                PantallaModoJuego::OpcionModo modo = pantallaModoJuego.obtenerOpcionConfirmada();

                if (modo == PantallaModoJuego::JUGADOR_VS_IA)
                {
                    esModoPvP = false;

                    estadoActual = EstadoJuego::DIFICULTAD_IA;
                }
                else if (modo == PantallaModoJuego::JUGADOR_VS_JUGADOR)
                {
                    esModoPvP = true;
                    
					pantallaMenu.detenerMusicaMenu();
                    sonidoJuego.reproducirAmbienteTablero();

                    estadoActual = EstadoJuego::TABLERO;
                }

                pantallaModoJuego.reiniciarConfirmacion();
            }

            continue;
        }

        if (estadoActual == EstadoJuego::DIFICULTAD_IA)
        {
            pantallaDificultadIA.procesarEventos();
            pantallaDificultadIA.actualizar();
            pantallaDificultadIA.dibujar();

            if (pantallaDificultadIA.debeVolverAlModoJuego())
            {
                pantallaDificultadIA.reiniciarVolver();
                estadoActual = EstadoJuego::MODO_JUEGO;
            }

            if (pantallaDificultadIA.estaOpcionConfirmada())
            {
                PantallaDificultadIA::Dificultad dificultad = pantallaDificultadIA.obtenerDificultadConfirmada();

                if (dificultad == PantallaDificultadIA::FACIL)
                {
                    cerebroIATablero.setDificultad(DificultadTablero::FACIL);
                }
                else if (dificultad == PantallaDificultadIA::NORMAL)
                {
                    cerebroIATablero.setDificultad(DificultadTablero::NORMAL);
                }
                else if (dificultad == PantallaDificultadIA::DIFICIL)
                {
                    cerebroIATablero.setDificultad(DificultadTablero::DIFICIL);
                }

                pantallaMenu.detenerMusicaMenu();
                sonidoJuego.reproducirAmbienteTablero();

                estadoActual = EstadoJuego::TABLERO;

                pantallaDificultadIA.reiniciarConfirmacion();
            }

            continue;
        }

        if (estadoActual == EstadoJuego::INSTRUCCIONES)
        {
            pantallaInstrucciones.procesarEventos();
            pantallaInstrucciones.actualizar();
            pantallaInstrucciones.dibujar();

            if (pantallaInstrucciones.debeVolverAlMenu())
            {
                pantallaInstrucciones.reiniciarVolver();
                estadoActual = EstadoJuego::MENU;
            }

            continue;
        }

        if (estadoActual == EstadoJuego::RANKING_PANTALLA)
        {
            pantallaRanking.procesarEventos();
            pantallaRanking.actualizar();
            pantallaRanking.dibujar();

            if (pantallaRanking.debeVolverAlMenu())
            {
                pantallaRanking.reiniciarVolver();
                estadoActual = EstadoJuego::MENU;
            }

            continue;
        }

        // --- EVENTOS DEL JUEGO REAL: TABLERO Y COMBATE ---

        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (estadoActual == EstadoJuego::TABLERO)
            {
                if (logicaTablero.getTurnoActual() == 1 || (logicaTablero.getTurnoActual() == 2 && esModoPvP))
                {
                    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
                    {
                        int clicX = static_cast<int>(event.mouseButton.x / (64 * graficosTablero.getEscala()));
                        int clicY = static_cast<int>(event.mouseButton.y / (64 * graficosTablero.getEscala()));

                        if (!logicaTablero.mover_Pieza(clicX, clicY))
                        {
                            logicaTablero.seleccionar_Pieza(clicX, clicY);
                        }
                    }
                }
            }
            else if (estadoActual == EstadoJuego::COMBATE) {
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


            }
        }

        // --- DIBUJADO Y ACTUALIZACION DEL JUEGO REAL ---

        window.clear();

        switch (estadoActual)
        {

        case EstadoJuego::TABLERO:
        {
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
                    sonidoJuego.reproducirEntradaCombate();
                    estadoActual = EstadoJuego::COMBATE;
                }
            }

            /*logicaTablero.Comprobar_Ganador();
            graficosTablero.dibujar(window, logicaTablero);
            break;
            */

            //////
            /////NUEVO 
            int ganador = logicaTablero.Comprobar_Ganador();

            if (ganador != 0)
            {
                equipoGanador = ganador;
                estadoActual = EstadoJuego::FIN_PARTIDA;
                break;
            }

            graficosTablero.dibujar(window, logicaTablero);
            break;
        }

        case EstadoJuego::COMBATE:
        {
            const float LIMITE_MIN_X = -25.f;
            const float LIMITE_MIN_Y = -25.f;
            const float LIMITE_MAX_X = 545.f;
            const float LIMITE_MAX_Y = 590.f;
            // --- ACTUALIZACIÓN JUGADOR 1 (LÍMITES TOTALMENTE COMPRIMIDOS AL ESTADIO AZUL) ---
            if (arenaCombate == nullptr || jugadorAzul == nullptr || jugadorRojo == nullptr)
            {
                estadoActual = EstadoJuego::TABLERO;
                break;
            }

            if (segadaActiva) {
                segadaTimer -= dt;
                float velocidadSegada = 520.f;
                float nuevaX = jugadorAzul->getHitbox().x + (dirSegada.x * velocidadSegada * dt);
                float nuevaY = jugadorAzul->getHitbox().y + (dirSegada.y * velocidadSegada * dt);
                ///ANTIGUO
                /*nuevaX = std::clamp(nuevaX, 25.f, 600.f - 25.f - jugadorAzul->getHitbox().ancho);
                nuevaY = std::clamp(nuevaY, 25.f, 560.f - 25.f - jugadorAzul->getHitbox().alto);*/
                //NUEVO
                nuevaX = std::clamp(nuevaX,LIMITE_MIN_X,LIMITE_MAX_X - jugadorAzul->getHitbox().ancho);
                nuevaY = std::clamp(nuevaY,LIMITE_MIN_Y,LIMITE_MAX_Y - jugadorAzul->getHitbox().alto);
                ///FIN NUEVO
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
                        std::clamp(jugadorAzul->getHitbox().x + (vA.x / len) * miVel * 100.f * dt, LIMITE_MIN_X, LIMITE_MAX_X - jugadorAzul->getHitbox().ancho),
                        std::clamp(jugadorAzul->getHitbox().y + (vA.y / len) * miVel * 100.f * dt, LIMITE_MIN_Y, LIMITE_MAX_Y - jugadorAzul->getHitbox().alto)
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
                        std::clamp(jugadorRojo->getHitbox().x + (vR.x / len) * velRed * 100.f * dt, LIMITE_MIN_X, LIMITE_MAX_X - jugadorAzul->getHitbox().ancho),
                        std::clamp(jugadorRojo->getHitbox().y + (vR.y / len) * velRed * 100.f * dt, LIMITE_MIN_Y, LIMITE_MAX_Y - jugadorAzul->getHitbox().alto));
                }
            }
            else {
                if (iaArena != nullptr)
                {
                    iaArena->actualizar(dt);
                }
            }

            arenaCombate->actualizar(dt);
            jugadorAzul->actualizar(dt);
            jugadorRojo->actualizar(dt);

            window.draw(sprCampo);

            sprLuchadorAzul.setScale(3.0f, 3.0f);
            sprLuchadorRojo.setScale(3.0f, 3.0f);
            sprBalon.setScale(0.8f, 0.8f);
            //ANTIGUO
            /*sprLuchadorAzul.setPosition(jugadorAzul->getHitbox().x, jugadorAzul->getHitbox().y);
            sprLuchadorRojo.setPosition(jugadorRojo->getHitbox().x, jugadorRojo->getHitbox().y);
            window.draw(sprLuchadorAzul);
            window.draw(sprLuchadorRojo);*/

            //NUEVO
            sprLuchadorAzul.setPosition(jugadorAzul->getHitbox().x, jugadorAzul->getHitbox().y);
            sprLuchadorRojo.setPosition(jugadorRojo->getHitbox().x, jugadorRojo->getHitbox().y);

            if (!jugadorAzul->estaMuerto())
            {
                window.draw(sprLuchadorAzul);
            }

            if (!jugadorRojo->estaMuerto())
            {
                window.draw(sprLuchadorRojo);
            }

            //
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

            //ANTIGUO
            /*if (arenaCombate->isTerminado() || jugadorAzul->estaMuerto() || jugadorRojo->estaMuerto()) {
                int bandoGanador = (jugadorAzul->estaMuerto()) ? 2 : 1;

                logicaTablero.aplicar_Resultado_Combate(origenX, origenY, combateDestX, combateDestY, bandoGanador);

                if (!esModoPvP && iaArena != nullptr)
                {
                    delete iaArena;
                    iaArena = nullptr;
                }


                delete arenaCombate; delete jugadorAzul; delete jugadorRojo;
                arenaCombate = nullptr;
                jugadorAzul = nullptr;
                jugadorRojo = nullptr;
                estadoActual = EstadoJuego::TABLERO;
            }*/

            //NUEVO
            if (!esperandoFinCombate &&
                (arenaCombate->isTerminado() || jugadorAzul->estaMuerto() || jugadorRojo->estaMuerto()))
            {
                esperandoFinCombate = true;
                timerFinCombate = TIEMPO_ESPERA_FIN_COMBATE;
                bandoGanadorCombate = (jugadorAzul->estaMuerto()) ? 2 : 1;
            }

            if (esperandoFinCombate)
            {
                timerFinCombate -= dt;

                if (timerFinCombate <= 0.0f)
                {
                    logicaTablero.aplicar_Resultado_Combate(
                        origenX, origenY,
                        combateDestX, combateDestY,
                        bandoGanadorCombate
                    );

                    if (!esModoPvP && iaArena != nullptr)
                    {
                        delete iaArena;
                        iaArena = nullptr;
                    }

                    delete arenaCombate;
                    delete jugadorAzul;
                    delete jugadorRojo;

                    arenaCombate = nullptr;
                    jugadorAzul = nullptr;
                    jugadorRojo = nullptr;

                    esperandoFinCombate = false;
                    timerFinCombate = 0.0f;
                    bandoGanadorCombate = 0;

                    estadoActual = EstadoJuego::TABLERO;
                }
            }

            break;
        }

        case EstadoJuego::FIN_PARTIDA:
        {
            window.clear(sf::Color(20, 20, 20));

            if (equipoGanador == 1)
            {
                textoFin.setString("HA GANADO EL EQUIPO AZUL");
            }
            else if (equipoGanador == 2)
            {
                textoFin.setString("HA GANADO EL EQUIPO ROJO");
            }

            window.draw(textoFin);
            window.draw(textoVolverMenu);

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
            {
                estadoActual = EstadoJuego::MENU;
                equipoGanador = 0;
            }

            break;
        }

        default:
            break;

        }
        window.display();

    }
    return 0;
}