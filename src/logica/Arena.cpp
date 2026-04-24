#include "Arena.h"
#pragma once
#include <vector>
#include "Jugador.h"
#include "Proyectil.h"

Arena::Arena(Jugador* j1, Jugador* j2) {
    jugador1 = j1;
    jugador2 = j2;
    // inicializo
    combateTerminado = false;
    ganador = nullptr;

    jugador1->setPosicion(100.0f, 300.0f);
    jugador2->setPosicion(700.0f, 300.0f);
}

// Detección de colisiones 
bool Arena::comprobarColision(Hitbox a, Hitbox b) {
    if (a.x < b.x + b.ancho &&
        a.x + a.ancho > b.x &&
        a.y < b.y + b.alto &&
        a.y + a.alto > b.y) {
        return true;
    }
    return false;
}

// añadir un balón a la lista 
void Arena::agregarProyectil(Proyectil p) {
    proyectiles.push_back(p);
}

void Arena::actualizar(float deltaTime) {
    if (combateTerminado) return;

    
 
    for (size_t i = 0; i < proyectiles.size(); ) {

        // muevo el balón
        proyectiles[i].actualizar(deltaTime);

        // Comprobamos si choca con el Jugador 1
        if (proyectiles[i].isActivo() && comprobarColision(proyectiles[i].getHitbox(), jugador1->getHitbox())) {
            jugador1->recibirDano(proyectiles[i].getDano());
            proyectiles[i].desactivar();
        }
        // Comprobamos si choca con el Jugador 2
        else if (proyectiles[i].isActivo() && comprobarColision(proyectiles[i].getHitbox(), jugador2->getHitbox())) {
            jugador2->recibirDano(proyectiles[i].getDano());
            proyectiles[i].desactivar();
        }

        // Si el balón se ha desactivado, lo borramos de la lista
        if (!proyectiles[i].isActivo()) {
            // erase elimina el elemento en esa posición y rueda los demás hacia atrás (Recomendacion de la IA)
            proyectiles.erase(proyectiles.begin() + i);
        }
        else {
            // Solo avanzamos la 'i' si NO hemos borrado el balón
            i++;
        }

    }

   
    // Comprobar muertes
    if (jugador1->estaMuerto()) {
        jugador1->setEstado(MUERTO);
        combateTerminado = true;
        ganador = jugador2;
    }
    else if (jugador2->estaMuerto()) {
        jugador2->setEstado(MUERTO); 
        combateTerminado = true;
        ganador = jugador1;
    }
}

void Arena::comandoDisparoJugador1(float dirX, float dirY) {
    if (jugador1->puedeAtacar()) {
        jugador1->setEstado(ATACANDO); 
        Proyectil nuevoBalon = jugador1->atacar(dirX, dirY);
        agregarProyectil(nuevoBalon);
        jugador1->reiniciarRecarga();
    }
}

void Arena::comandoDisparoJugador2(float dirX, float dirY) {
    if (jugador2->puedeAtacar()) {
        jugador2->setEstado(ATACANDO);
        Proyectil nuevoBalon = jugador2->atacar(dirX, dirY);
        agregarProyectil(nuevoBalon);
        jugador2->reiniciarRecarga();
    }
}

void Arena::comandoMoverJugador1(float dirX, float dirY, float deltaTime) {
    // Comprobamos si el jugador está parado o moviéndose
    if (dirX == 0.0f && dirY == 0.0f) {
        jugador1->setEstado(QUIETO);
    }
    else {
        jugador1->setEstado(CAMINANDO);

        // El movimiento y límites
        float desplazamientoX = dirX * jugador1->getVelocidad() * deltaTime;
        float desplazamientoY = dirY * jugador1->getVelocidad() * deltaTime;
        jugador1->mover(desplazamientoX, desplazamientoY);
    }

    // LÍMITES DEL CAMPO 
    Hitbox caja = jugador1->getHitbox();
    if (caja.x < 0.0f) jugador1->setPosicion(0.0f, caja.y);
    if (caja.x + caja.ancho > 800.0f) jugador1->setPosicion(800.0f - caja.ancho, caja.y);

    caja = jugador1->getHitbox();
    if (caja.y < 0.0f) jugador1->setPosicion(caja.x, 0.0f);
    if (caja.y + caja.alto > 600.0f) jugador1->setPosicion(caja.x, 600.0f - caja.alto);
}

void Arena::comandoMoverJugador2(float dirX, float dirY, float deltaTime) {
    // Lo mismo para el jugador 2
    if (dirX == 0.0f && dirY == 0.0f) {
        jugador2->setEstado(QUIETO);
    }
    else {
        jugador2->setEstado(CAMINANDO);

        float desplazamientoX = dirX * jugador2->getVelocidad() * deltaTime;
        float desplazamientoY = dirY * jugador2->getVelocidad() * deltaTime;
        jugador2->mover(desplazamientoX, desplazamientoY);
    }

    Hitbox caja = jugador2->getHitbox();
    if (caja.x < 0.0f) jugador2->setPosicion(0.0f, caja.y);
    if (caja.x + caja.ancho > 800.0f) jugador2->setPosicion(800.0f - caja.ancho, caja.y);

    caja = jugador2->getHitbox();
    if (caja.y < 0.0f) jugador2->setPosicion(caja.x, 0.0f);
    if (caja.y + caja.alto > 600.0f) jugador2->setPosicion(caja.x, 600.0f - caja.alto);
}

void Arena::comandoHabilidadJugador1() {
    jugador1->usarHabilidadEspecial();
    jugador1->setEstado(ATACANDO);
}

void Arena::comandoHabilidadJugador2() {
    jugador2->usarHabilidadEspecial();
    jugador2->setEstado(ATACANDO);
}