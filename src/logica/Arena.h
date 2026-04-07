#pragma once
#include "Jugador.h"
#include <vector>
#include "Proyectil.h"

class Arena {
private:
    Jugador* jugador1; 
    Jugador* jugador2; 

    //hago una lista de vectores que guarde todos los balones activos en el campo:
    std::vector <Proyectil> proyectiles;


    bool combateTerminado;
    Jugador* ganador;

public:
    // Al crearse la arena, recibe a los dos jugadores que chocaron
    Arena(Jugador* j1, Jugador* j2);


    void agregarProyectil(Proyectil p);

    void actualizar(float deltaTime);
    bool comprobarColision(Hitbox a, Hitbox b);
    bool isTerminado() const { return combateTerminado; }
    Jugador* getGanador() const { return ganador; }

};