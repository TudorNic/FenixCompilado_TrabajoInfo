#pragma once
#include "Jugador.h"

class Entrenador : public Jugador {
private:
    int cargasMagia = 2;
public:
    Entrenador() : Jugador(80, 3.0f, 20, 1.0f) { hitbox.ancho = 45.f; hitbox.alto = 90.f; }
    std::string getNombreClase() const override { return "entrenador"; }
    void atacar(std::vector<Proyectil>& proy, Jugador* op, float dx, float dy, int id) override {
        if (!puedeAtacar()) return;
        proy.push_back(Proyectil(hitbox.x + hitbox.ancho / 2 - 10, hitbox.y + hitbox.alto / 2 - 10, dx * 420, dy * 420, danoAtaque, id));
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