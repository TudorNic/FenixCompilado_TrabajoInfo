#pragma once
#include "Jugador.h"

class Entrenador : public Jugador {
private:
    int cargasMagia = 2;

public:
    Entrenador() : Jugador(80, 3.0f, 20, 1.0f) {
        hitbox.ancho = 45.0f; hitbox.alto = 90.0f;
    }

    void atacar(std::vector<Proyectil>& proyectiles, Jugador* oponente, float dx, float dy, int id) override {
        if (!puedeAtacar()) return;
        proyectiles.push_back(Proyectil(hitbox.x + hitbox.ancho / 2 - 10, hitbox.y + hitbox.alto / 2 - 10, dx * 420, dy * 420, danoAtaque, id));
        reiniciarRecarga();
    }

    void usarHabilidadEspecial() override {
        if (cargasMagia > 0 && vidaActual < vidaMaxima) {
            vidaActual += (int)(vidaMaxima * 0.4f);
            if (vidaActual > vidaMaxima) vidaActual = vidaMaxima;
            cargasMagia--;
        }
    }
};