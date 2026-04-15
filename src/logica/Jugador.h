#pragma once
#include "Proyectil.h"

enum EstadoJugador {
    QUIETO,
    CAMINANDO,
    ATACANDO,
    MUERTO
};

class Jugador {
protected:
    int vidaMaxima;
    int vidaActual;
    float velocidad;
    int danoAtaque;

    // Sistema de recarga
    float tiempoRecarga;
    float tiempoRecargaActual; // Cronómetro que bajará hasta 0

    Hitbox hitbox; // Ya sabe lo que es porque viene de Proyectil.h
    EstadoJugador estadoActual;

public:
    Jugador(int vida, float vel, int dano, float recarga)
        : vidaMaxima(vida), vidaActual(vida), velocidad(vel), danoAtaque(dano), tiempoRecarga(recarga), tiempoRecargaActual(0.0f), estadoActual(QUIETO) {
    }

    virtual ~Jugador() = default;

    //La función atacar ahora recibe hacia dónde chutamos y devuelve un balón
    virtual Proyectil atacar(float dirX, float dirY) = 0;
    virtual void usarHabilidadEspecial() {
        // Por defecto no hace nada, para que piezas como el Defensa no crasheen
    }
    // El jugador también necesita actualizar su cronómetro 
    virtual void actualizar(float deltaTime) {
        if (tiempoRecargaActual > 0.0f) {
            tiempoRecargaActual -= deltaTime;
        }
    }

    bool puedeAtacar() const { return tiempoRecargaActual <= 0.0f; }
    void reiniciarRecarga() { tiempoRecargaActual = tiempoRecarga; }

    EstadoJugador getEstado() const { return estadoActual; }
    void setEstado(EstadoJugador nuevoEstado) { estadoActual = nuevoEstado; }

    Hitbox getHitbox() const { return hitbox; }
    void setPosicion(float nx, float ny) { hitbox.x = nx; hitbox.y = ny; }
    void mover(float dx, float dy) { hitbox.x += dx; hitbox.y += dy; }
    void recibirDano(int cantidad) { vidaActual -= cantidad; }
    bool estaMuerto() const { return vidaActual <= 0; }
    int getVidaActual() const { return vidaActual; }
    float getVelocidad() const { return velocidad; }
};