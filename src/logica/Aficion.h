#pragma once
#include "Jugador.h"
#include "Proyectil.h"
class Aficion : public Jugador {
public:
    // Stats: Vida muy baja (30), velocidad media-alta (4.0f), daño bajo (10), recarga rápida (0.4f)
    Aficion() : Jugador(30, 4.0f, 10, 0.4f) {
        hitbox.ancho = 15.0f;
        hitbox.alto = 15.0f;
    }

    Proyectil atacar(float dirX, float dirY) override {
        // Proyectil a 200 de velocidad
        return Proyectil(hitbox.x, hitbox.y, dirX * 200.0f, dirY * 200.0f, danoAtaque);
    }
};