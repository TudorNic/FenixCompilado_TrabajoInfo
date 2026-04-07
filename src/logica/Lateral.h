#pragma once
#include "Jugador.h"

class Lateral : public Jugador {
public:
    // Stats: Vida media-baja (60), bastante rápido (4.5f), daño moderado (12), recarga rápida (0.6f)
    Lateral() : Jugador(60, 4.5f, 12, 0.6f) {
        // Un tamaño de hitbox intermedio
        hitbox.ancho = 18.0f;
        hitbox.alto = 18.0f;
    }

    void atacar() override {
        // Lógica de ataque para el lateral (ej. lanzar el balón con efecto)
    }
};