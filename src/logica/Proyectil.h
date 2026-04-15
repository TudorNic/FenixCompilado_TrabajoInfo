#pragma once

// Definimos la hitbox aquí arriba
struct Hitbox {
    float x, y, ancho, alto;
};

class Proyectil {
private:
    Hitbox hitbox;
    float velocidadX;
    float velocidadY;
    int dano;
    bool activo; // Para saber si el balón sigue en juego

public:
    // para poder cambiar de forma segura los datos 
    Proyectil(float startX, float startY, float velX, float velY, int danoAtaque) {
        hitbox.x = startX;
        hitbox.y = startY;
        hitbox.ancho = 10.0f; // Tamaño estándar del balón
        hitbox.alto = 10.0f;

        velocidadX = velX;
        velocidadY = velY;
        dano = danoAtaque;
        activo = true;
    }

    // actualizacion del balon en tiempo real
    void actualizar(float deltaTime) {
        if (activo) {
            hitbox.x += velocidadX * deltaTime;
            hitbox.y += velocidadY * deltaTime;
        }
    }

    // Getters y Setters
    Hitbox getHitbox() const { return hitbox; }
    int getDano() const { return dano; }
    bool isActivo() const { return activo; }
    void desactivar() { activo = false; } // Se llama cuando choca con alguien o sale del límite
};