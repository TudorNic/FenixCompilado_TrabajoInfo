#pragma once
#include "Jugador.h"
#include "Pieza.h"
#include<iostream>

class Defensa : public Jugador, public Pieza {
public:
    Defensa(int bando) : Jugador(100, 200.0f, 25, 1.5f), Pieza("Defensa", 80, 15, 0.8, 2, Forma_Movimiento::Tierra, bando) {
        hitbox.ancho = 50.f;
        hitbox.alto = 100.f;
        }
    std::string getNombreClase() const override { return "defensa"; }
    void atacar(std::vector<Proyectil>& proy, Jugador* op, float dx, float dy, int id) override {
        if (!puedeAtacar()) return;
        proy.push_back(Proyectil(hitbox.x + hitbox.ancho / 2 - 10, hitbox.y + hitbox.alto / 2 - 10, dx * 450, dy * 450, danoAtaque, id));
        reiniciarRecarga();
        activarAnimacionAtaque(); // Disparo visual
    }
};