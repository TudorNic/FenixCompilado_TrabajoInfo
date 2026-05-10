#pragma once
#include "Jugador.h"

class Lateral : public Jugador {
public:
    // Stats: 60 Vida, 4.5f Velocidad, 12 Daño, 0.6f Recarga
    Lateral(int equipo) : Jugador(60, 4.5f, 12, 0.6f,1,equipo) {
        hitbox.ancho = 40.0f;
        hitbox.alto = 80.0f;
    }
    std::string getNombreClase() const override { return "lateral"; }
    void atacar(std::vector<Proyectil>& proyectiles, Jugador* oponente, float dirX, float dirY, int miID) override {
        if (!puedeAtacar()) return;

        float spawnX = hitbox.x + (hitbox.ancho / 2.0f) - 10.0f;
        float spawnY = hitbox.y + (hitbox.alto / 2.0f) - 10.0f;

        proyectiles.push_back(Proyectil(spawnX, spawnY, dirX * 500.0f, dirY * 500.0f, danoAtaque, miID));

        reiniciarRecarga();
        activarAnimacionAtaque();
    }
};