#pragma once
#include <SFML/Graphics.hpp>
#include "../Logica/Tablero.h"
#include "../Logica/Jugador.h"
class TableroGrafico
{
    sf::Texture texGrass;    
    sf::Texture texPower;  
    sf::Texture texEntrenador[2]; 
    sf::Texture texDelantero[2];
    sf::Texture texCentrocampista[2];
    sf::Texture texDefensa[2];
    sf::Texture texLateral[2];
    sf::Texture texAficion[2];
    sf::Sprite spriteTile;   
    sf::Sprite spriteJugador;
    float escala;            

public:
    TableroGrafico(float escalaDeseada);

    void cargarTexturas();
   
    void dibujar(sf::RenderWindow& window, Tablero& logica);

    void dibujarPieza(sf::RenderWindow& window, int tipo, int bando, int col, int fila);

    void dibujarEntidades(sf::RenderWindow& window, Jugador &j,int col,int fila);

    float getEscala() const { return escala; };
};

