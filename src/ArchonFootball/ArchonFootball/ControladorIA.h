#pragma once
#include "Jugador.h"
#include "Arena.h"

class ControladorIA {
private:
    Jugador* bot;       // Puntero al jugador controlado por la CPU
    Jugador* humano;    // Puntero al jugador rival
    Arena* arena;       // Puntero a la arena para poder ejecutar movimientos

public:
    // Constructor
    ControladorIA(Jugador* botPtr, Jugador* humanoPtr, Arena* arenaPtr);

    // Función que se llamará en cada frame
    void actualizar(float deltaTime);
};