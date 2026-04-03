#pragma once
#include "Jugador.h"

class Defensa : public Jugador {
public:
    // Stats: Vida muy alta (100), velocidad baja (2.0f), daño alto (25), recarga lenta (1.5f)
    Defensa() : Jugador(100, 2.0f, 25, 1.5f) {
        hitbox.ancho = 25.0f;
        hitbox.alto = 25.0f;
    }

    void atacar() override {
        // Lógica de ataque fuerte a corta/media distancia (se me ocurre un pelotazo fuerte o una entrada)
    }
};