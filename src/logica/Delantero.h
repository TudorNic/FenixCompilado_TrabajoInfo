#pragma once
#include "Jugador.h"
#include "Proyectil.h"

class Delantero : public Jugador {
public:
    Delantero() : Jugador(50, 5.0f, 15, 0.5f) {
        hitbox.ancho = 20.0f;
        hitbox.alto = 20.0f;
    }

    
};