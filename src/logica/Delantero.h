#pragma once
#include "Jugador.h"
#include "Pieza.h"
#include <iostream>
class Delantero : public Jugador,public Pieza {
public:
    Delantero(int bando) : Jugador(50, 5.0f, 15, 0.5f), Pieza("Delantero", 120, 10, 0.5, 1, Forma_Movimiento::Tierra, bando) {
        hitbox.ancho = 40.f;
        hitbox.alto = 80.f; }

    std::string getNombreClase() const override { return "delantero"; }

    void atacar(std::vector<Proyectil>& proy, Jugador* op, float dx, float dy, int id) override {
        if (!puedeAtacar()) return;
        proy.push_back(Proyectil(hitbox.x + hitbox.ancho / 2 - 10, hitbox.y + hitbox.alto / 2 - 10, dx * 550, dy * 550, danoAtaque, id));
        reiniciarRecarga();
        activarAnimacionAtaque();
    }
};