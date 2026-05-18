#include "IA_Tablero.h"
#include "Jugador.h"
#include <iostream>

void IATablero::ejecutarTurno(Tablero& tablero) {
    // Evita bucles de movimiento infinitos entre dos casillas
    static int lastOrigenX = -1, lastOrigenY = -1;
    static int lastDestX = -1, lastDestY = -1;
    std::vector<MovimientoIA> mejoresMovimientos;

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            Jugador* p = tablero.getCasilla(i, j);

            if (p != nullptr && p->getBando() == bandoIA) {

                int radio = p->getRadio();
                for (int dx = -radio; dx <= radio; dx++) {
                    for (int dy = -radio; dy <= radio; dy++) {


                        if (dx == 0 && dy == 0) continue;

                        int destX = i + dx;
                        int destY = j + dy;
                        if (tablero.Verificar_Movimiento(i, j, destX, destY)) {
                            int puntosDestino = calcularPuntuacion(tablero, destX, destY, p);
                            int puntosOrigen = calcularPuntuacion(tablero, i, j, p);

                            int mejora = puntosDestino - puntosOrigen;

                            // Prioridad de movimiento si el entrenador está amenazado
                            if (p->getNombreClase() == "entrenador" && puntosOrigen < -1000) {
                                mejora += 10000; 
                            }
                            // Penalización por repetición de jugada
                            if (i == lastDestX && j == lastDestY && destX == lastOrigenX && destY == lastOrigenY) {
                                mejora -= 1000;
                            }
                            mejoresMovimientos.push_back({ i, j, destX, destY, mejora });
                        }
                    }
                }
            }
        }
    }

    if (!mejoresMovimientos.empty()) {
        int maxPuntuacion = -999999;
        for (const auto& m : mejoresMovimientos) {
            if (m.puntuacion > maxPuntuacion) {
                maxPuntuacion = m.puntuacion;
            }
        }

        // Selección entre las mejores opciones disponibles
        std::vector<MovimientoIA> jugadasTop;
        for (const auto& m : mejoresMovimientos) {
            if (m.puntuacion >= maxPuntuacion - 3) {
                jugadasTop.push_back(m);
            }
        }


        MovimientoIA mejor = jugadasTop[rand() % jugadasTop.size()];

        // Simulación de error para niveles bajos
        if (dificultad == DificultadTablero::FACIL && mejoresMovimientos.size() > 1) {
            if (rand() % 100 < 30) {
                mejor = mejoresMovimientos[rand() % mejoresMovimientos.size()];
            }
        }

        lastOrigenX = mejor.xOrigen;
        lastOrigenY = mejor.yOrigen;

        lastDestX = mejor.xDestino;
        lastDestY = mejor.yDestino;

        tablero.seleccionar_Pieza(mejor.xOrigen, mejor.yOrigen);
        tablero.mover_Pieza(mejor.xDestino, mejor.yDestino);
    }
}

int IATablero::calcularPuntuacion(Tablero& tablero, int x, int y, Jugador* p) {
        float puntuacion = 0;

        // Control de posición respecto al centro
        float distCentro = std::sqrt(std::pow(x - 4, 2) + std::pow(y - 4, 2));
        float factorCentro = (10.0f - distCentro);

        bool esPoder = esPuntoDePoder(x, y);

        switch (dificultad) {
        case DificultadTablero::FACIL:
            puntuacion += (rand() % 40);
            if (bandoIA == 2) puntuacion += (y * 3.0f);
            else puntuacion += ((8 - y) * 3.0f);
            puntuacion += factorCentro * 2.0f;
            break;

        case DificultadTablero::NORMAL:
        {
            puntuacion += factorCentro * 4;
            if (esPoder) puntuacion += 120;

            Jugador* objetivo = tablero.getCasilla(x, y);
            if (objetivo != nullptr && objetivo->getBando() != bandoIA) {
                if (p->getVidaActual() > objetivo->getVidaActual()) puntuacion += 100;
                else puntuacion -= 50;
            }

            if (bandoIA == 2) puntuacion += (y * 2);
            else puntuacion += ((8 - y) * 2);
        }
            break;

        case DificultadTablero::DIFICIL:
        {
            if (esPoder) puntuacion += 200;
            puntuacion += factorCentro * 3;

            Jugador* enemigo = tablero.getCasilla(x, y);
            std::string miClase = p->getNombreClase();

            // Análisis de objetivos y riesgos de combate
            if (enemigo != nullptr && enemigo->getBando() != bandoIA) {
                std::string claseEnemiga = enemigo->getNombreClase();
                if (miClase == "aficion" && (claseEnemiga == "entrenador" || claseEnemiga == "Portero" || enemigo->getVidaActual() > 70)) {
                    puntuacion += 150;
                }
                else if (miClase == "entrenador") {
                    if (enemigo->getVidaActual() > 20) puntuacion -= 500;
                    else puntuacion += 100;
                }
                else {
                    if (p->getVidaActual() > enemigo->getVidaActual()) puntuacion += 80;
                    if (enemigo->getVidaActual() < 30) puntuacion += 100;
                }
            }

            // Evaluación de amenazas en área
            bool casillaEvaluadaAmenazada = false;
            bool esAtaqueAlAgresor = false;
            int origX = p->getX();
            int origY = p->getY();

            for (int i = 0; i < 9; i++) {
                for (int j = 0; j < 9; j++) {
                    Jugador* rival = tablero.getCasilla(i, j);

                    if (rival != nullptr && rival->getBando() != bandoIA) {
                        int distRivalX = std::abs(x - i);
                        int distRivalY = std::abs(y - j);
                        int distanciaReal = std::max(distRivalX, distRivalY);

                        if (distanciaReal <= rival->getRadio()) {
                            if (miClase == "entrenador" || rival->getVidaActual() >= p->getVidaActual()) {
                                casillaEvaluadaAmenazada = true;
                            }
                        }

                        // Posibilidad de contraataque
                        int distDesdeOrigen = std::max(std::abs(origX - i), std::abs(origY - j));
                        if (distDesdeOrigen <= rival->getRadio()) {
                            if (x == i && y == j) {
                                if (p->getVidaActual() > rival->getVidaActual() || rival->getVidaActual() < 25) {
                                    esAtaqueAlAgresor = true;
                                }
                            }
                        }
                    }
                }
            }

            if (casillaEvaluadaAmenazada) {
                if (miClase == "entrenador") puntuacion -= 5000;
                else puntuacion -= 600;
            }

            if (esAtaqueAlAgresor) {
                puntuacion += 7000;
            }
            // Reglas tácticas por clase
            if (miClase == "defensa") {
                if (bandoIA == 2 && y <= 3) puntuacion += 50;
                else if (bandoIA == 1 && y >= 5) puntuacion += 50;

                for (int dx = -1; dx <= 1; dx++) {
                    for (int dy = -1; dy <= 1; dy++) {
                        int nx = x + dx, ny = y + dy;
                        if (nx >= 0 && nx < 9 && ny >= 0 && ny < 9) {
                            Jugador* aliado = tablero.getCasilla(nx, ny);
                            if (aliado != nullptr && aliado->getBando() == bandoIA && aliado->getNombreClase() == "entrenador") {
                                puntuacion += 80;
                            }
                        }
                    }
                }
            }
            else if (miClase == "delantero" || miClase == "aficion") {
                if (bandoIA == 2) puntuacion += (y * 2);
                else puntuacion += ((8 - y) * 2);
            }
        }
            break;
        }

        return static_cast<int>(puntuacion);
    
}


bool IATablero::esPuntoDePoder(int x, int y) {
    if (x == 4 && y == 4) return true; // Centro
    if (x == 4 && y == 0) return true; // Norte
    if (x == 4 && y == 8) return true; // Sur
    if (x == 0 && y == 4) return true; // Oeste
    if (x == 8 && y == 4) return true; // Este
    return false;
}