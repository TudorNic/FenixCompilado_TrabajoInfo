#pragma once
#include "tablero.h"
#include <vector>
enum class DificultadTablero { FACIL, NORMAL, DIFICIL };

struct MovimientoIA {
    int xOrigen, yOrigen;
    int xDestino, yDestino;
    int puntuacion;
};

class IATablero {
private:
    int bandoIA;
    int calcularPuntuacion(Tablero& tablero, int x, int y, Jugador* p);
    bool esPuntoDePoder(int x, int y);
    DificultadTablero dificultad;

public:

    IATablero(int bando) : bandoIA(bando), dificultad(DificultadTablero::NORMAL) {}
    void setDificultad(DificultadTablero dif) { dificultad = dif; }
    DificultadTablero getDificultad() const { return dificultad; }
    void ejecutarTurno(Tablero& tablero);
};