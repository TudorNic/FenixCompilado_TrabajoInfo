#pragma once
#include "Jugador.h"

class Delantero : public Jugador {
public:
    // Stats: 50 Vida, 5.0f Velocidad, 15 Daño, 0.5f Recarga
    Delantero() : Jugador(50, 300.0f, 15, 0.5f) { // 300.0f de velocidad
        hitbox.ancho = 40.0f;
        hitbox.alto = 80.0f;
    }

    void atacar(std::vector<Proyectil>& proyectiles, Jugador* oponente, float dirX, float dirY, int miID) override {
        if (!puedeAtacar()) return;

        float spawnX = hitbox.x + (hitbox.ancho / 2.0f) - 10.0f;
        float spawnY = hitbox.y + (hitbox.alto / 2.0f) - 10.0f;

        // Sus balones van más rápido (550.0f)
        proyectiles.push_back(Proyectil(spawnX, spawnY, dirX * 550.0f, dirY * 550.0f, danoAtaque, miID));

        reiniciarRecarga();
    }
};