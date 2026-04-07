#pragma once
#include "Jugador.h"

class Centrocampista : public Jugador {
public:
    // Stats: Muy equilibrado. Vida buena (75), velocidad media (3.5f), daño sólido (18), recarga media (0.8f)
    Centrocampista() : Jugador(75, 3.5f, 18, 0.8f) {
        // Un tamaño de hitbox un poco más grandote
        hitbox.ancho = 22.0f;
        hitbox.alto = 22.0f;
    }

    void atacar() override {
        // Lógica de ataque para el centrocampista (ej. un pase tenso o disparo preciso)
    }
};