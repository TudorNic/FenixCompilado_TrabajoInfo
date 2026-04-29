#include "Arena.h"
#include <algorithm>

Arena::Arena(Jugador* j1, Jugador* j2) {
    jugador1 = j1;
    jugador2 = j2;
    combateTerminado = false;
    ganador = nullptr;
}

bool Arena::comprobarColision(Hitbox a, Hitbox b) {
    return (a.x < b.x + b.ancho &&
        a.x + a.ancho > b.x &&
        a.y < b.y + b.alto &&
        a.y + a.alto > b.y);
}

void Arena::agregarProyectil(Proyectil p) {
    proyectiles.push_back(p);
}

void Arena::actualizar(float deltaTime) {
    // 1. Mover proyectiles SIEMPRE (evita que se congelen al morir alguien)
    for (size_t i = 0; i < proyectiles.size(); ) {
        proyectiles[i].actualizar(deltaTime);

        // Colisión con J1 (si el balón no es suyo)
        if (proyectiles[i].isActivo() && proyectiles[i].getIdDueno() != 1 &&
            comprobarColision(proyectiles[i].getHitbox(), jugador1->getHitbox())) {
            jugador1->recibirDano(proyectiles[i].getDano());
            proyectiles[i].desactivar();
        }
        // Colisión con J2 (si el balón no es suyo)
        else if (proyectiles[i].isActivo() && proyectiles[i].getIdDueno() != 2 &&
            comprobarColision(proyectiles[i].getHitbox(), jugador2->getHitbox())) {
            jugador2->recibirDano(proyectiles[i].getDano());
            proyectiles[i].desactivar();
        }

        // Borrado de seguridad si salen del mapa
        if (proyectiles[i].getHitbox().x < -100 || proyectiles[i].getHitbox().x > 2500)
            proyectiles[i].desactivar();

        if (!proyectiles[i].isActivo()) proyectiles.erase(proyectiles.begin() + i);
        else i++;
    }

    if (combateTerminado) return; // La lógica de muerte se detiene, pero los balones ya se movieron arriba

    if (jugador1->estaMuerto()) { combateTerminado = true; ganador = jugador2; }
    else if (jugador2->estaMuerto()) { combateTerminado = true; ganador = jugador1; }
}

void Arena::comandoDisparoJugador1(float dx, float dy) {
    if (!combateTerminado) {
        jugador1->atacar(proyectiles, jugador2, dx, dy, 1);
    }
}

void Arena::comandoDisparoJugador2(float dx, float dy) {
    if (!combateTerminado) {
        jugador2->atacar(proyectiles, jugador1, dx, dy, 2);
    }
}