#include "tablerografico.h"
#include "direct.h"
#include <iostream>

bool cargarTextura(sf::Texture& textura, const std::string& ruta)
{
    if (!textura.loadFromFile(ruta))
    {
        std::cout << "ERROR cargando textura: " << ruta << std::endl;
        return false;
    }

    std::cout << "Textura cargada correctamente: " << ruta << std::endl;
    return true;

   
}




TableroGrafico::TableroGrafico(float escalaDeseada) : escala(escalaDeseada) {
}

void TableroGrafico::cargarTexturas() {
    // Carga de balones y césped
    texGrass.loadFromFile("assets/board/tile_grass.png");
    texPower.loadFromFile("assets/board/tile_power.png");

    // --- EQUIPO AZUL (Bando 0) - Corregidos guiones a "-1.png" ---
    texDelantero[0].loadFromFile("assets/players/blue/delantero/atack/sprite_delantero_blue_atack_1.png");
    texDefensa[0].loadFromFile("assets/players/blue/defensa/atack/sprite_defensa_blue_atack-1.png");
    texEntrenador[0].loadFromFile("assets/players/blue/entrenador/atack/sprite_entrenador_blue_atack-1.png");
    texCentrocampista[0].loadFromFile("assets/players/blue/centrocampista/atack/sprite_centrocampista_blue_atack-1.png");
    texLateral[0].loadFromFile("assets/players/blue/portero/atack/sprite_portero_blue_atack-1.png");
    //texLateral[0].loadFromFile("assets/players/blue/defensa/atack/sprite_defensa_blue_atack-1.png");
    texAficion[0].loadFromFile("assets/players/blue/aficion/atack/sprite_aficion_blue_atack-1.png");

    // --- EQUIPO ROJO (Bando 1) - Corregidos guiones a "-1.png" ---
    texDelantero[1].loadFromFile("assets/players/red/delantero/atack/sprite_delantero_red_atack-1.png");
    texDefensa[1].loadFromFile("assets/players/red/defensa/atack/sprite_defensa_red_atack-1.png");
    texEntrenador[1].loadFromFile("assets/players/red/entrenador/atack/sprite_entrenador_red_atack-1.png");
    texCentrocampista[1].loadFromFile("assets/players/red/centrocampista/atack/sprite_centrocampista_red_atack-1.png");
    texLateral[1].loadFromFile("assets/players/red/portero/atack/sprite_portero_red_atack-1.png");
   // texLateral[1].loadFromFile("assets/players/red/defensa/atack/sprite_defensa_red_atack-1.png");
   // texAficion[1].loadFromFile("assets/players/red/aficion/atack/sprite_aficion_red_atack-1.png");
    texAficion[1].loadFromFile("../../../assets/players/red/aficion/atack/sprite_aficion_red_atack-1.png");
    //cargarTextura(texAficion[1], "../../../assets/players/red/aficion/atack/sprite_aficion_red_atack-1.png");
    spriteTile.setTexture(texGrass);
    spriteTile.setScale(escala, escala);
}

void TableroGrafico::dibujar(sf::RenderWindow& window, Tablero& logica) {
    const float TAM_TILE = 64.f;

    sf::RectangleShape rejilla(sf::Vector2f(64 * escala, 64 * escala));
    rejilla.setFillColor(sf::Color::Transparent);
    rejilla.setOutlineColor(sf::Color(255, 255, 255, 60));
    rejilla.setOutlineThickness(1.0f);

    // Dibujar el campo verde
    for (int fila = 0; fila < 9; fila++) {
        for (int col = 0; col < 9; col++) {
            spriteTile.setColor(sf::Color::White);
            spriteTile.setTexture(texGrass);

            int efecto = logica.getEfecto_Casilla(col, fila);

            if (efecto == 1) {
                spriteTile.setColor(sf::Color(255, 215, 0, 110));
            }
            else if (efecto == 2) {
                spriteTile.setColor(sf::Color(255, 215, 50, 180));
            }

            spriteTile.setPosition(col * TAM_TILE * escala, fila * TAM_TILE * escala);
            spriteTile.setScale(escala, escala);
            window.draw(spriteTile);

            rejilla.setPosition(col * 64 * escala, fila * 64 * escala);
            window.draw(rejilla);
        }
    }

    // Dibujar puntos de poder (Balones animados)
    for (int i = 0; i < 5; i++) {
        int px = logica.getPuntoPoder(i).x;
        int py = logica.getPuntoPoder(i).y;

        spriteTile.setTexture(texPower);
        spriteTile.setScale(escala * 0.6f, escala * 0.6f);
        float offset = (64 * escala * (1.0f - 0.6f)) / 2.0f;
        spriteTile.setPosition((px * 64 * escala) + offset, (py * 64 * escala) + offset);
        window.draw(spriteTile);
    }

    // Dibujar las piezas de los jugadores si la casilla contiene uno
    for (int x = 0; x < 9; x++) {
        for (int y = 0; y < 9; y++) {
            Jugador* j = logica.getCasilla(x, y);
            if (j != nullptr) {
                dibujarEntidades(window, *j, x, y);
            }
        }
    }
}

void TableroGrafico::dibujarPieza(sf::RenderWindow& window, int tipo, int bando, int col, int fila) {
    if (tipo == 0) spriteJugador.setTexture(texEntrenador[bando]);
    else if (tipo == 1) spriteJugador.setTexture(texDefensa[bando]);
    else if (tipo == 2) spriteJugador.setTexture(texDelantero[bando]);
    else if (tipo == 3) spriteJugador.setTexture(texCentrocampista[bando]);
    else if (tipo == 4) spriteJugador.setTexture(texLateral[bando]);
    else if (tipo == 5) spriteJugador.setTexture(texAficion[bando]);

    spriteJugador.setScale(escala, escala);
    float x = col * 64 * escala;
    float y = fila * 64 * escala;
    float tamCelda = 64.0f * escala;

    float anchoSprite = spriteJugador.getGlobalBounds().width;
    float altoSprite = spriteJugador.getGlobalBounds().height;

    float offsetX = (tamCelda - anchoSprite) / 2.0f;
    float offsetY = (tamCelda - altoSprite) / 2.0f;
    spriteJugador.setPosition(x + offsetX, y + offsetY);
    window.draw(spriteJugador);
}

void TableroGrafico::dibujarEntidades(sf::RenderWindow& window, Jugador& j, int col, int fila) {
    int bando = j.getBando() - 1;
    std::string nombre = j.getNombreClase();
    int tipoVisual = 5; 

    if (nombre == "entrenador")          tipoVisual = 0;
    else if (nombre == "defensa")        tipoVisual = 1;
    else if (nombre == "delantero")      tipoVisual = 2;
    else if (nombre == "centrocampista") tipoVisual = 3;
    else if (nombre == "lateral")        tipoVisual = 4;
    else if (nombre == "aficion")        tipoVisual = 5;

    dibujarPieza(window, tipoVisual, bando, col, fila);
}