#pragma once
#include "Jugador.h"
#include "Arena.h"
#include "Proyectil.h"

// Definimos los estados de la IA
enum class EstadoIA {
    PERSEGUIR,
    ESQUIVAR
};

class ControladorIA {
private:
    Jugador* bot;       // Puntero al jugador controlado por la CPU
    Jugador* humano;    // Puntero al jugador rival
    Arena* arena;       // Puntero a la arena para poder ejecutar movimientos
    EstadoIA estadoActual;

    bool detectarPeligro(float& dirEscapeX, float& dirEscapeY);

public:
    // Constructor
    ControladorIA(Jugador* botPtr, Jugador* humanoPtr, Arena* arenaPtr);
    // Función que se llamará en cada frame
    void actualizar(float deltaTime);
};