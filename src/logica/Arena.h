#pragma once
#include <vector>
#include "Proyectil.h"

class Jugador; // Forward declaration para evitar bucles

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

    // --- Asegúrate de que estas 4 estén aquí ---
    void comandoDisparoJugador1(float dx, float dy);
    void comandoDisparoJugador2(float dx, float dy);
    void comandoEspecialJugador1(); // <--- Nueva
    void comandoEspecialJugador2(); // <--- Nueva

    bool isTerminado() const { return combateTerminado; }
    Jugador* getGanador() const { return ganador; }
    const std::vector<Proyectil>& getProyectiles() const { return proyectiles; }
};