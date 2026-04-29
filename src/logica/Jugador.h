#pragma once
#include "Proyectil.h"

enum EstadoJugador { QUIETO, CAMINANDO, ATACANDO, MUERTO };

class Jugador {
protected:
    int vidaMaxima, vidaActual;
    float velocidad, tiempoRecarga, tiempoRecargaActual;
    int danoAtaque;
    Hitbox hitbox; // De Proyectil.h
    EstadoJugador estadoActual;

public:
    Jugador(int vida, float vel, int dano, float recarga)
        : vidaMaxima(vida), vidaActual(vida), velocidad(vel), danoAtaque(dano),
        tiempoRecarga(recarga), tiempoRecargaActual(0.0f), estadoActual(QUIETO) {
    }

    virtual ~Jugador() = default;

    virtual Proyectil atacar(float dirX, float dirY, int miID) {
        // Calculamos el centro matemático de la caja de colisión
        float centroX = hitbox.x + (hitbox.ancho / 2.0f);
        float centroY = hitbox.y + (hitbox.alto / 2.0f);

        // El proyectil nace centrado (ajustamos -10 para que el centro del balón coincida)
        return Proyectil(centroX - 10.0f, centroY - 10.0f, dirX * 450.0f, dirY * 450.0f, danoAtaque, miID);
    }

    virtual void usarHabilidadEspecial() {}

    virtual void actualizar(float deltaTime) {
        if (tiempoRecargaActual > 0.0f) tiempoRecargaActual -= deltaTime;
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
    float getVelocidad() const { return velocidad; }
    int getVidaActual() const { return vidaActual; }
    int getVidaMaxima() const { return vidaMaxima; }
};