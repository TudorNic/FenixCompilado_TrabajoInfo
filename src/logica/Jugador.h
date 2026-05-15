#pragma once
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include "Proyectil.h"

enum EstadoJugador { QUIETO, CAMINANDO, ATACANDO, MUERTO };

class Jugador {
protected:
    int vidaMaxima, vidaActual;
    float velocidad, tiempoRecarga, tiempoRecargaActual;
    int danoAtaque;
    Hitbox hitbox;
    EstadoJugador estadoActual;
    float tiempoAnimacion = 0.0f;
    int frameActual = 0;
    float timerAtaqueVisual = 0.0f;
    float cooldownEspecial = 5.0f;
    float tiempoEspecialActual = 0.0f;
    const float DURACION_ATAQUE = 0.2f;
    int bando;
    int radio;
    int pos_x, pos_y;

public:
    Jugador(int vida, float vel, int dano, float recarga,int rad,int equipo)
        : vidaMaxima(vida), vidaActual(vida), velocidad(vel), danoAtaque(dano),
        tiempoRecarga(recarga),bando(equipo),radio(rad), tiempoRecargaActual(0.0f), estadoActual(QUIETO) {
        hitbox.x = 0.0f; hitbox.y = 0.0f; hitbox.ancho = 0.0f; hitbox.alto = 0.0f; pos_x = 0; pos_y = 0;
    }

    virtual ~Jugador() = default;
    virtual void atacar(std::vector<Proyectil>& proyectiles, Jugador* oponente, float dirX, float dirY, int miID) = 0;
    virtual std::string getNombreClase() const = 0;
    virtual void usarHabilidadEspecial() {}

    void activarAnimacionAtaque() {
        estadoActual = ATACANDO;
        timerAtaqueVisual = DURACION_ATAQUE;
    }

    virtual void actualizar(float deltaTime) {
        if (tiempoRecargaActual > 0.0f) tiempoRecargaActual -= deltaTime;
        if (tiempoEspecialActual > 0.0f) tiempoEspecialActual -= deltaTime;
        if (timerAtaqueVisual > 0.0f) {
            timerAtaqueVisual -= deltaTime;
            if (timerAtaqueVisual <= 0.0f) {
                estadoActual = QUIETO;
            }
        }

        if (estadoActual == CAMINANDO) {
            tiempoAnimacion += deltaTime;
            if (tiempoAnimacion >= 0.15f) {
                frameActual = (frameActual + 1) % 3;
                tiempoAnimacion = 0.0f;
            }
        }
        else if (estadoActual != ATACANDO) {
            frameActual = 0;
        }
    }

    void establecer_Posicion(int x, int y) {
        pos_x = x;
        pos_y = y;
    }

    int getFrameActual() const { return frameActual; }
    EstadoJugador getEstado() const { return estadoActual; }
    void setEstado(EstadoJugador nuevo) {
        if (estadoActual != ATACANDO) estadoActual = nuevo;
    }
    int getVidaActual() const { return vidaActual; }
    int getVidaMaxima() const { return vidaMaxima; }
    void recibirDano(int cant) { vidaActual -= cant; if (vidaActual < 0) vidaActual = 0; }
    bool estaMuerto() const { return vidaActual <= 0; }
    Hitbox getHitbox() const { return hitbox; }
    void setPosicion(float nx, float ny) { hitbox.x = nx; hitbox.y = ny; }
    bool puedeAtacar() const { return tiempoRecargaActual <= 0.0f; }
    void reiniciarRecarga() { tiempoRecargaActual = tiempoRecarga; }
    float getVelocidad() const { return velocidad; }
    float getDanoAtaque()const { return danoAtaque; }
    bool puedeUsarEspecial() const { return tiempoEspecialActual <= 0.0f; }
    void reiniciarEspecial() { tiempoEspecialActual = cooldownEspecial; }
    int getBando() const { return bando; }
    int getRadio() const { return radio; }
    int getX() const { return pos_x; }
    int getY() const { return pos_y; }
};