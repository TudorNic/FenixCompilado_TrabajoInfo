#pragma once
#include "Jugador.h"
#include "Proyectil.h"
class Entrenador : public Jugador {
public:
    // Stats: Vida alta (80), velocidad media (3.0f), daño medio-alto (20), recarga media (1.0f)
    Entrenador() : Jugador(80, 3.0f, 20, 1.0f) {
        hitbox.ancho = 20.0f;
        hitbox.alto = 20.0f;
    }

    Proyectil atacar(float dirX, float dirY) override {
        // Tiro estándar, luego le añado la magia
        return Proyectil(hitbox.x, hitbox.y, dirX * 250.0f, dirY * 250.0f, danoAtaque);
    }

    // El líder tiene habilidades extra q hare mas adelante
    void usarHabilidadEspecial() {
        // Lógica de hechizo/táctica
    }
};