#pragma once
#include <vector>
#include "Jugador.h"
#include "Proyectil.h"

class Arena {
private:
    Jugador* jugador1;
    Jugador* jugador2;

    std::vector<Proyectil> proyectiles;

    bool combateTerminado;
    Jugador* ganador;

public:
    Arena(Jugador* j1, Jugador* j2);

    void actualizar(float deltaTime);
    bool comprobarColision(Hitbox a, Hitbox b);

    void agregarProyectil(Proyectil p);
    void comandoDisparoJugador1(float dirX, float dirY);
    void comandoDisparoJugador2(float dirX, float dirY);

    bool isTerminado() const { return combateTerminado; }
    Jugador* getGanador() const { return ganador; }
};