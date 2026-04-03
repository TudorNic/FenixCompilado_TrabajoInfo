#include "Arena.h"

Arena::Arena(Jugador* j1, Jugador* j2) {
    jugador1 = j1;
    jugador2 = j2;
    //inicializo
    combateTerminado = false;
    ganador = nullptr;

    jugador1->setPosicion(100.0f, 300.0f);
    jugador2->setPosicion(700.0f, 300.0f);
}

// Detección de colisiones (revisar)
bool Arena::comprobarColision(Hitbox a, Hitbox b) {
    if (a.x < b.x + b.ancho &&
        a.x + a.ancho > b.x &&
        a.y < b.y + b.alto &&
        a.y + a.alto > b.y) {
        return true;
    }
    return false;
}

void Arena::actualizar(float deltaTime) {
    if (combateTerminado) return;

    // Aquí metere la lógica de los balones más adelante

    // Comprobar muertes
    if (jugador1->estaMuerto()) {
        combateTerminado = true;
        ganador = jugador2;
    }
    else if (jugador2->estaMuerto()) {
        combateTerminado = true;
        ganador = jugador1;
    }
}