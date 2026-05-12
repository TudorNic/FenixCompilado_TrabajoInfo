#include "IA_Tablero.h"
#include "Pieza.h"
#include <iostream>

void IATablero::ejecutarTurno(Tablero& tablero) {
    std::vector<MovimientoIA> mejoresMovimientos;

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            Pieza* p = tablero.getCasilla(i, j);

            if (p != nullptr && p->getBando() == bandoIA) {
                //Simulamos movimientos en un radio (según la pieza)
                int radio = p->getradio();
                for (int dx = -radio; dx <= radio; dx++) {
                    for (int dy = -radio; dy <= radio; dy++) {
                        int destX = i + dx;
                        int destY = j + dy;
                        if (tablero.Verificar_Movimiento(i, j, destX, destY)) {
                            int puntos = calcularPuntuacion(tablero, destX, destY, p);
                            mejoresMovimientos.push_back({ i, j, destX, destY, puntos });
                        }
                    }
                }
            }
        }
    }

    //Elegir el movimiento con más puntos (el más inteligente)
    if (!mejoresMovimientos.empty()) {
        MovimientoIA mejor = mejoresMovimientos[0];
        for (const auto& m : mejoresMovimientos) {
            if (m.puntuacion > mejor.puntuacion) mejor = m;
        }
        tablero.seleccionar_Pieza(mejor.xOrigen, mejor.yOrigen);
        tablero.mover_Pieza(mejor.xDestino, mejor.yDestino);
    }
}

int IATablero::calcularPuntuacion(Tablero& tablero, int x, int y, Pieza* p) {
    int puntos = 0;

    // Regla 1: Priorizar Puntos de Poder
    if (esPuntoDePoder(x, y)) puntos += 100;

    // Regla 2: Si hay un enemigo, evaluar si nos conviene atacar
    Pieza* enemigo = tablero.getCasilla(x, y);
    if (enemigo != nullptr && enemigo->getBando() != bandoIA) {
        puntos += 50;
        if (enemigo->getNombre() == "Portero") puntos += 100;
    }

    // Regla 3: Bonus por acercarse al centro (4,4)
    int distanciaAlCentro = abs(4 - x) + abs(4 - y);
    puntos += (10 - distanciaAlCentro);

    return puntos;
}

bool IATablero::esPuntoDePoder(int x, int y) {
    if (x == 4 && y == 4) return true; // Centro
    if (x == 4 && y == 0) return true; // Norte
    if (x == 4 && y == 8) return true; // Sur
    if (x == 0 && y == 4) return true; // Oeste
    if (x == 8 && y == 4) return true; // Este
    return false;
}