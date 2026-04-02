#pragma once
#include "Jugador.h"

class Arena {
private:
    Jugador* jugador1; 
    Jugador* jugador2; 

    bool combateTerminado;
    Jugador* ganador;

public:
    // Al crearse la arena, recibe a los dos jugadores que chocaron
    Arena(Jugador* j1, Jugador* j2);

    void actualizar(float deltaTime);
    bool comprobarColision(Hitbox a, Hitbox b);
};