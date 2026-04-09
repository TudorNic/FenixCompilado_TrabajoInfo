#pragma once
#include "Jugador.h"
#include "Proyectil.h"

class Centrocampista : public Jugador {
public:
    // Stats: Muy equilibrado. Vida buena (75), velocidad media (3.5f), daño sólido (18), recarga media (0.8f)
    Centrocampista() : Jugador(75, 3.5f, 18, 0.8f) {
        // Un tamaño de hitbox un poco más grandote
        hitbox.ancho = 22.0f;
        hitbox.alto = 22.0f;
    }
    Proyectil atacar(float dirX, float dirY) override {
        // Balón a 250 de velocidad (tiro tenso y controlado)
        return Proyectil(hitbox.x, hitbox.y, dirX * 250.0f, dirY * 250.0f, danoAtaque);
    }
};