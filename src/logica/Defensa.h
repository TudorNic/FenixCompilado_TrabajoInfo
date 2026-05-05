#pragma once
#include "Jugador.h"

class Defensa : public Jugador {
public:
    // Stats: 100 Vida, 2.0f Velocidad, 25 Daño, 1.5f Recarga
    Defensa() : Jugador(100, 150.0f, 25, 1.5f) { // 150.0f de velocidad
        hitbox.ancho = 50.0f;
        hitbox.alto = 100.0f;
    }

    void atacar(std::vector<Proyectil>& proyectiles, Jugador* oponente, float dirX, float dirY, int miID) override {
        if (!puedeAtacar()) return;

        // Creamos el balón y lo metemos directamente en la lista que nos pasa la Arena
        float spawnX = hitbox.x + (hitbox.ancho / 2.0f) - 10.0f;
        float spawnY = hitbox.y + (hitbox.alto / 2.0f) - 10.0f;

        proyectiles.push_back(Proyectil(spawnX, spawnY, dirX * 450.0f, dirY * 450.0f, danoAtaque, miID));

        reiniciarRecarga();
    }
};