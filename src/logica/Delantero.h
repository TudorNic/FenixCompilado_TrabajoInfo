#pragma once
#include "Jugador.h"

class Delantero : public Jugador {
public:
    // Inicializamos con sus stats: poca vida (50), muy rápido (5.0f), daño medio (15), recarga rápida (0.5s)
    Delantero() : Jugador(50, 5.0f, 15, 0.5f) {
        // Tamaño de la hitbox 
        hitbox.ancho = 20.0f;
        hitbox.alto = 20.0f;
    }

    void atacar() override {
        // programar logica de ataque mas adelante. He pensado en q lance balones como proyectiles
    }
};