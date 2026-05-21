#include "Arena.h"
#include "Jugador.h"
#include "Proyectil.h"

Arena::Arena(Jugador* j1, Jugador* j2)
    : jugador1(j1), jugador2(j2), combateTerminado(false), ganador(nullptr) {
}

bool Arena::comprobarColision(Hitbox a, Hitbox b) {
    return (a.x < b.x + b.ancho && a.x + a.ancho > b.x &&
        a.y < b.y + b.alto && a.y + a.alto > b.y);
}

void Arena::actualizar(float deltaTime) {
    for (size_t i = 0; i < proyectiles.size(); ) {
        proyectiles[i].actualizar(deltaTime);
        if (proyectiles[i].isActivo() && proyectiles[i].getIdDueno() != 1 && comprobarColision(proyectiles[i].getHitbox(), jugador1->getHitbox())) {
            jugador1->recibirDano(proyectiles[i].getDano());
            proyectiles[i].desactivar();
        }
        else if (proyectiles[i].isActivo() && proyectiles[i].getIdDueno() != 2 && comprobarColision(proyectiles[i].getHitbox(), jugador2->getHitbox())) {
            jugador2->recibirDano(proyectiles[i].getDano());
            proyectiles[i].desactivar();
        }

        /*if (!proyectiles[i].isActivo() || proyectiles[i].getHitbox().x < -100 || proyectiles[i].getHitbox().x > 2500) {
            proyectiles.erase(proyectiles.begin() + i);
        }*/
        Hitbox hb = proyectiles[i].getHitbox();

        if (!proyectiles[i].isActivo() ||
            hb.x < 0 || hb.x > 560 ||
            hb.y < 0 || hb.y > 560)
        {
            proyectiles.erase(proyectiles.begin() + i);
        }
        else i++;
    }

    if (combateTerminado) return;
    if (jugador1->estaMuerto()) { combateTerminado = true; ganador = jugador2; }
    else if (jugador2->estaMuerto()) { combateTerminado = true; ganador = jugador1; }
}

void Arena::comandoDisparoJugador1(float dx, float dy) {
    if (!combateTerminado) jugador1->atacar(proyectiles, jugador2, dx, dy, 1);
}

void Arena::comandoDisparoJugador2(float dx, float dy) {
    if (!combateTerminado) jugador2->atacar(proyectiles, jugador1, dx, dy, 2);
}

void Arena::comandoEspecialJugador1() {
    if (!combateTerminado) jugador1->usarHabilidadEspecial();
}

void Arena::comandoEspecialJugador2() {
    if (!combateTerminado) jugador2->usarHabilidadEspecial();
}