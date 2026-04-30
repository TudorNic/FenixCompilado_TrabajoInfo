#pragma once
#include <vector>
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

    // MÉTODO CLAVE: Ahora cada clase decide si genera un proyectil o pega cuerpo a cuerpo
    virtual void atacar(std::vector<Proyectil>& proyectiles, Jugador* oponente, float dirX, float dirY, int miID) = 0;

    virtual void usarHabilidadEspecial() {} // Por defecto no hace nada (solo Entrenador la usa)

    virtual void actualizar(float deltaTime) {
        if (tiempoRecargaActual > 0.0f) tiempoRecargaActual -= deltaTime;
    }

    // Getters y Setters
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