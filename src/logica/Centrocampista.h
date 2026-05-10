#pragma once
#include "Jugador.h"

class Centrocampista : public Jugador {
public:
    // Stats: 75 Vida, 3.5f Velocidad, 18 Daño, 0.8f Recarga
    Centrocampista(int equipo) : Jugador(75, 3.5f, 18, 0.8f,1,equipo) {
        hitbox.ancho = 45.0f;
        hitbox.alto = 90.0f;
    }
    std::string getNombreClase() const override { return "centrocampista"; }
    void atacar(std::vector<Proyectil>& proyectiles, Jugador* oponente, float dirX, float dirY, int miID) override {
        if (!puedeAtacar()) return;

        float spawnX = hitbox.x + (hitbox.ancho / 2.0f) - 10.0f;
        float spawnY = hitbox.y + (hitbox.alto / 2.0f) - 10.0f;

        proyectiles.push_back(Proyectil(spawnX, spawnY, dirX * 480.0f, dirY * 480.0f, danoAtaque, miID));

        reiniciarRecarga();
        activarAnimacionAtaque();
    }
};