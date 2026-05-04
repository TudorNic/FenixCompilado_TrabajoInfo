#pragma once
#include <vector>
#include <string>
#include <cmath>
#include "Proyectil.h"

enum EstadoJugador { QUIETO, CAMINANDO, ATACANDO, MUERTO };

class Jugador {
protected:
    int vidaMaxima, vidaActual;
    float velocidad, tiempoRecarga, tiempoRecargaActual;
    int danoAtaque;
    Hitbox hitbox;
    EstadoJugador estadoActual;

public:
    Jugador(int vida, float vel, int dano, float recarga)
        : vidaMaxima(vida), vidaActual(vida), velocidad(vel), danoAtaque(dano),
        tiempoRecarga(recarga), tiempoRecargaActual(0.0f), estadoActual(QUIETO) {
    }

    virtual ~Jugador() = default;

    // Obligatorio para todas las clases
    virtual void atacar(std::vector<Proyectil>& proyectiles, Jugador* oponente, float dirX, float dirY, int miID) = 0;
    virtual std::string getNombreClase() const = 0;
    virtual void usarHabilidadEspecial() {}

    virtual void actualizar(float deltaTime) {
        if (tiempoRecargaActual > 0.0f) tiempoRecargaActual -= deltaTime;
    }

    // Getters / Setters
    bool puedeAtacar() const { return tiempoRecargaActual <= 0.0f; }
    void reiniciarRecarga() { tiempoRecargaActual = tiempoRecarga; }
    int getVidaActual() const { return vidaActual; }
    int getVidaMaxima() const { return vidaMaxima; }
    void recibirDano(int cant) { vidaActual -= cant; if (vidaActual < 0) vidaActual = 0; }
    bool estaMuerto() const { return vidaActual <= 0; }
    Hitbox getHitbox() const { return hitbox; }
    void setPosicion(float nx, float ny) { hitbox.x = nx; hitbox.y = ny; }
    float getVelocidad() const { return velocidad; }
};