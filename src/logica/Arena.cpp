#include "Arena.h"

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

   
   
    if (jugador1->estaMuerto()) {
        combateTerminado = true;
        ganador = jugador2;
    }
    else if (jugador2->estaMuerto()) {
        combateTerminado = true;
        ganador = jugador1;
    }
}