#pragma once
#include "Jugador.h"
#include "Proyectil.h"

class Delantero : public Jugador {
public:
    Delantero() : Jugador(50, 5.0f, 15, 0.5f) {
        hitbox.ancho = 20.0f;
        hitbox.alto = 20.0f;
    }

    // El delantero crea un balón y se lo da a la Arena
    Proyectil atacar(float dirX, float dirY) override {
        // La velocidad del balón del delantero es muy alta (400.0f)
       
        // Creamos el proyectil justo en la posición donde está el delantero
        return Proyectil(hitbox.x, hitbox.y, dirX * 400.0f, dirY * 400.0f, danoAtaque);
    }
};