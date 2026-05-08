#pragma once
#include "tablero.h"
#include <vector>

struct MovimientoIA {
    int xOrigen, yOrigen;
    int xDestino, yDestino;
    int puntuacion;
};

class IATablero {
private:
    int bandoIA;
    int calcularPuntuacion(Tablero& tablero, int x, int y, Pieza* p);
    bool esPuntoDePoder(int x, int y);

public:
    IATablero(int bando) : bandoIA(bando) {}
    void ejecutarTurno(Tablero& tablero);
};