#pragma once
#include "Jugador.h"

class Entrenador : public Jugador {
private:
    int cargasMagia; 

public:
    Entrenador() : Jugador(80, 3.0f, 20, 1.0f) {
        hitbox.ancho = 20.0f;
        hitbox.alto = 20.0f;
        cargasMagia = 2; // 2 usos por combate
    }

    void usarHabilidadEspecial() override {
        if (cargasMagia > 0) {
            vidaActual += (vidaMaxima * 0.4f); // Cura un 40%
            if (vidaActual > vidaMaxima) vidaActual = vidaMaxima; // Tope de vida
            cargasMagia--;
        }
    }

    Proyectil atacar(float dirX, float dirY) override {
        return Proyectil(hitbox.x, hitbox.y, dirX * 250.0f, dirY * 250.0f, danoAtaque);
    }
};