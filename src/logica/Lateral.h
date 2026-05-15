#pragma once
#include "Jugador.h"

class Lateral : public Jugador {
public:
    // Stats: 60 Vida, 4.5f Velocidad, 12 Daño, 0.6f Recarga
    Lateral(int equipo) : Jugador(60, 4.5f, 12, 0.6f, 4, equipo) {
        hitbox.ancho = 40.0f;
        hitbox.alto = 80.0f;
    }
}