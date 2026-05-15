#include "tablerografico.h"
#include"direct.h"
#include <iostream>

TableroGrafico::TableroGrafico(float escalaDeseada) : escala(escalaDeseada) {

}

void TableroGrafico::cargarTexturas() {
    texGrass.loadFromFile("assets/board/tile_grass.png");
    texPower.loadFromFile("assets/board/tile_power.png");

    texDelantero[0].loadFromFile("assets/players/blue/delantero/atack/sprite_delantero_blue_atack_1.png");
    texDefensa[0].loadFromFile("assets/players/blue/defensa/atack/sprite_defensa_blue_atack-1.png");
    texEntrenador[0].loadFromFile("assets/players/blue/entrenador/atack/sprite_entrenador_blue_atack-1.png");
    texCentrocampista[0].loadFromFile("assets/players/blue/centrocampista/atack/sprite_centrocampista_blue_atack-1.png");
    texLateral[0].loadFromFile("assets/players/blue/portero/atack/sprite_portero_blue_atack-1.png"); 
    texAficion[0].loadFromFile("assets/players/blue/aficion/atack/sprite_aficion_blue_atack-1.png"); 
    

    texDelantero[1].loadFromFile("assets/players/red/delantero/atack/sprite_delantero_red_atack-1.png");
    texDefensa[1].loadFromFile("assets/players/red/defensa/atack/sprite_defensa_red_atack-1.png");
    texEntrenador[1].loadFromFile("assets/players/red/entrenador/atack/sprite_entrenador_red_atack-1.png");
    texCentrocampista[1].loadFromFile("assets/players/red/centrocampista/atack/sprite_centrocampista_red_atack-1.png");
    texLateral[1].loadFromFile("assets/players/red/portero/atack/sprite_portero_red_atack-1.png");
    texAficion[1].loadFromFile("assets/players/red/aficion/atack/sprite_aficion_red_atack_1.png");
    spriteTile.setTexture(texGrass);
    // Configuramos el escalado inicial
    spriteTile.setScale(escala, escala);
}

void TableroGrafico::dibujar(sf::RenderWindow& window, Tablero& logica) {
    // El tamaño de tus tiles suele ser 64x64. Con escala 3.0, cada uno mide 192px.
    const float TAM_TILE = 64.f;

    sf::RectangleShape rejilla(sf::Vector2f(64 * escala, 64 * escala));
    rejilla.setFillColor(sf::Color::Transparent);
    rejilla.setOutlineColor(sf::Color(255, 255, 255, 60)); // Blanco suave
    rejilla.setOutlineThickness(1.0f);

    //Campo verde
    for (int fila = 0; fila < 9; fila++)
    {
        for (int col = 0; col < 9; col++)
        {
            spriteTile.setColor(sf::Color::White);
            spriteTile.setTexture(texGrass);
            spriteTile.setPosition(col * TAM_TILE * escala, fila * TAM_TILE * escala);
            spriteTile.setScale(escala, escala);
            window.draw(spriteTile);

            rejilla.setPosition(col * 64 * escala, fila * 64 * escala);
            window.draw(rejilla);
        }
    }

    //Puntos de poder
    for (int i = 0; i < 5; i++)
    {


        int px = logica.getPuntoPoder(i).x;
        int py = logica.getPuntoPoder(i).y;

        spriteTile.setTexture(texPower);
        spriteTile.setScale(escala * 0.6f, escala * 0.6f);
        float offset = (64 * escala * (1.0f - 0.6f)) / 2.0f;
        spriteTile.setPosition((px * 64 * escala)+offset,( py * 64 * escala )+offset);
        window.draw(spriteTile);

   
    }

    for (int f = 0; f < 9; f++) {
        for (int c = 0; c < 9; c++) {
            // Obtenemos el puntero de esa casilla
            Jugador* j = logica.getCasilla(f, c);

            // ¡IMPORTANTE! Solo dibujamos si la casilla NO está vacía
            if (j != nullptr) {

                dibujarEntidades(window, *j,f,c); // Pasamos el jugador (desreferenciado)
            }
        }
    }
   
}

void TableroGrafico::dibujarPieza(sf::RenderWindow& window, int tipo, int bando, int col, int fila)
{
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
    spriteJugador.setPosition( x+ offsetX, y  + offsetY);
    window.draw(spriteJugador);
}

void TableroGrafico::dibujarEntidades(sf::RenderWindow& window, Jugador& j,int col,int fila)
{
    int x= col;
    int y= fila;
    int bando = j.getBando() - 1;
    int tipoVisual;

    if (y == 0 || y == 8)
    { // Usamos 'y' para las filas de atrás
        if (x == 4)
        {
            tipoVisual = 0; dibujarPieza(window, tipoVisual, bando, x, y);
        }         // Entrenador
        else
            if (x == 3 || x == 5)
            {
                tipoVisual = 1;
                dibujarPieza(window, tipoVisual, bando, x, y);

            }// Defensa
            else
                if (x == 2 || x == 6 || x == 1 || x == 7)
                {
                    tipoVisual = 3;
                    dibujarPieza(window, tipoVisual, bando, x, y);
                } // Centrocampista
                else
                    if (x == 0 || x == 8)
                    {
                        tipoVisual = 4; 
                        dibujarPieza(window, tipoVisual, bando, x, y);

                    }// laterales

    }
    else 
        if (y == 1 || y == 7) 
        {
            if(x==0 || x==8) {
                tipoVisual = 2;
                dibujarPieza(window, tipoVisual, bando, x, y);
            }
            else if(x >= 1 && x<=7) {
                tipoVisual = 5;
                dibujarPieza(window, tipoVisual, bando, x, y);
            }
        }
}
         
    
