#pragma once

struct Hitbox {
    float x, y, ancho, alto;
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

    Hitbox hitbox;

public:
    Jugador(int vida, float vel, int dano, float recarga)
        : vidaMaxima(vida), vidaActual(vida), velocidad(vel), danoAtaque(dano), tiempoRecarga(recarga), tiempoRecargaActual(0.0f) {
    }

    virtual ~Jugador() = default;

    //La función atacar ahora recibe hacia dónde chutamos y devuelve un balón
    virtual Proyectil atacar(float dirX, float dirY) = 0;
    //preparamos el ataque especial del entrenador
    // 
    // El jugador también necesita actualizar su cronómetro 
    virtual void actualizar(float deltaTime) {
        if (tiempoRecargaActual > 0.0f) {
            tiempoRecargaActual -= deltaTime;
        }
    }

    bool puedeAtacar() const { return tiempoRecargaActual <= 0.0f; }
    void reiniciarRecarga() { tiempoRecargaActual = tiempoRecarga; }

 
    Hitbox getHitbox() const { return hitbox; }
    void setPosicion(float nx, float ny) { hitbox.x = nx; hitbox.y = ny; }
    void mover(float dx, float dy) { hitbox.x += dx; hitbox.y += dy; }
    void recibirDano(int cantidad) { vidaActual -= cantidad; }
    bool estaMuerto() const { return vidaActual <= 0; }
    int getVidaActual() const { return vidaActual; }
};