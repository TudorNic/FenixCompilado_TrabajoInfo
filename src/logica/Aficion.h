#pragma once
#include "Jugador.h"

class Aficion : public Jugador {
public:
    Aficion() : Jugador(40, 4.5f, 20, 0.4f) { hitbox.ancho = 40.f; hitbox.alto = 70.f; }
    std::string getNombreClase() const override { return "aficion"; }
    void atacar(std::vector<Proyectil>& proy, Jugador* op, float dx, float dy, int id) override {
        if (!puedeAtacar()) return;
        float oX = op->getHitbox().x + op->getHitbox().ancho / 2;
        float oY = op->getHitbox().y + op->getHitbox().alto / 2;
        float dist = std::sqrt(std::pow(oX - (hitbox.x + hitbox.ancho / 2), 2) + std::pow(oY - (hitbox.y + hitbox.alto / 2), 2));
        if (dist < 85.0f) { op->recibirDano(danoAtaque); reiniciarRecarga(); }
    }
};