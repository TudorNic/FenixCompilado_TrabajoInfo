#pragma once
#include "Jugador.h"
#include "Proyectil.h"

class Defensa : public Jugador {
public:
    // Stats: Vida muy alta (100), velocidad baja (2.0f), daño alto (25), recarga lenta (1.5f)
    Defensa() : Jugador(100, 2.0f, 25, 1.5f) {
        hitbox.ancho = 25.0f;
        hitbox.alto = 25.0f;
    }

    Proyectil atacar(float dirX, float dirY) override {
        // Balón a 150 de velocidad (lento, pero quita mucha vida)
        return Proyectil(hitbox.x, hitbox.y, dirX * 150.0f, dirY * 150.0f, danoAtaque);
    }
};