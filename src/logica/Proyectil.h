#pragma once

struct Hitbox {
    float x, y, ancho, alto;
};

class Proyectil {
private:
    Hitbox hitbox;
    float velocidadX;
    float velocidadY;
    int dano;
    bool activo;
    int idDueno;

public:
    Proyectil(float startX, float startY, float velX, float velY, int danoAtaque, int ownerID) {
        hitbox.x = startX;
        hitbox.y = startY;
        hitbox.ancho = 20.0f;
        hitbox.alto = 20.0f;
        velocidadX = velX;
        velocidadY = velY;
        dano = danoAtaque;
        activo = true;
        idDueno = ownerID;
    }

    void actualizar(float deltaTime) {
        if (activo) {
            hitbox.x += velocidadX * deltaTime;
            hitbox.y += velocidadY * deltaTime;
        }
    }

    Hitbox getHitbox() const { return hitbox; }
    int getDano() const { return dano; }
    bool isActivo() const { return activo; }
    int getIdDueno() const { return idDueno; }
    void desactivar() { activo = false; }
};