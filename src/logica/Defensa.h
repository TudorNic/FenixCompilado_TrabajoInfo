#pragma once
#include "Jugador.h"
#include "Proyectil.h"

class Defensa : public Jugador {
public:
    // Stats: Vida 100, velocidad 2.0f, daño 25, recarga 1.5f
    Defensa() : Jugador(100, 2.0f, 25, 1.5f) {
        // Ajustamos la caja para que cubra el cuerpo del sprite
        hitbox.ancho = 50.0f;
        hitbox.alto = 100.0f;
    }
};