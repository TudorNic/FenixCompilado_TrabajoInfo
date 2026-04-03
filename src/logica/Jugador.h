#pragma once

// Estructura matemática para colisiones y posición 
struct Hitbox {
    float x, y, ancho, alto;
};

// CLASE BASE
class Jugador {
protected:
    int vidaMaxima;
    int vidaActual;
    float velocidad;
    int danoAtaque;
    float tiempoRecarga; // Cadencia de ataque 
    Hitbox hitbox;

public:
    // Constructor
    Jugador(int vida, float vel, int dano, float recarga)
        : vidaMaxima(vida), vidaActual(vida), velocidad(vel), danoAtaque(dano), tiempoRecarga(recarga) {
    }

    // Destructor virtual
    virtual ~Jugador() = default;

    
    virtual void atacar() = 0;

    // Métodos comunes para el motor de la Arena
    Hitbox getHitbox() const { return hitbox; }
    void setPosicion(float nx, float ny) { hitbox.x = nx; hitbox.y = ny; }
    void mover(float dx, float dy) { hitbox.x += dx; hitbox.y += dy; }
    void recibirDano(int cantidad) { vidaActual -= cantidad; }
    bool estaMuerto() const { return vidaActual <= 0; }
    int getVidaActual() const { return vidaActual; }
};