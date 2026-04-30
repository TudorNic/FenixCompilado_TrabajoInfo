#pragma once
#include "Jugador.h"

class Aficion : public Jugador {
public:
    Aficion() : Jugador(40, 4.5f, 20, 0.4f) { // Muy rápida pero poca vida
        hitbox.ancho = 40.0f;
        hitbox.alto = 70.0f;
    }

    void atacar(std::vector<Proyectil>& proyectiles, Jugador* oponente, float dx, float dy, int id) override {
        if (!puedeAtacar()) return;

        // Calculamos distancia al centro del oponente
        float centroX = hitbox.x + hitbox.ancho / 2;
        float centroY = hitbox.y + hitbox.alto / 2;
        float oX = oponente->getHitbox().x + oponente->getHitbox().ancho / 2;
        float oY = oponente->getHitbox().y + oponente->getHitbox().alto / 2;

        float dist = std::sqrt(std::pow(oX - centroX, 2) + std::pow(oY - centroY, 2));

        if (dist < 85.0f) { // Rango de golpe corto
            oponente->recibirDano(danoAtaque);
            reiniciarRecarga();
        }
    }
};