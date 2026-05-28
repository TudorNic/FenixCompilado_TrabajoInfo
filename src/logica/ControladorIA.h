#pragma once
#include "Jugador.h"
#include "Arena.h"
#include "Proyectil.h"

enum class DificultadIA {
    FACIL,
    NORMAL,
    DIFICIL
};

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

    DificultadIA dificultadActual;
    bool detectarPeligro(float& dirEscapeX, float& dirEscapeY);

    float temporizadorInicio;

public:
    // Constructor
    ControladorIA(Jugador* botPtr, Jugador* humanoPtr, Arena* arenaPtr);

    void setDificultad(DificultadIA nivel) { dificultadActual = nivel; }
    void actualizar(float deltaTime);
};