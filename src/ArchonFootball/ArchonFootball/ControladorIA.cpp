#include "ControladorIA.h"
#include <cmath> // Necesario para std::sqrt

ControladorIA::ControladorIA(Jugador* botPtr, Jugador* humanoPtr, Arena* arenaPtr) {
    bot = botPtr;
    humano = humanoPtr;
    arena = arenaPtr;
}

void ControladorIA::actualizar(float deltaTime) {
    // Si alguien ya ha muerto o el combate terminó, la IA no hace nada
    if (arena->isTerminado() || bot->estaMuerto() || humano->estaMuerto()) {
        return;
    }

    Hitbox hBot = bot->getHitbox();
    Hitbox hHumano = humano->getHitbox();

    // 1. Calcular la diferencia de posiciones
    float dx = hHumano.x - hBot.x;
    float dy = hHumano.y - hBot.y;

    // 2. Calcular la distancia real entre los dos
    float distancia = std::sqrt(dx * dx + dy * dy);

    // 3. Moverse hacia el jugador (si no estamos ya encima)
    float dirX = 0.0f;
    float dirY = 0.0f;

    if (distancia > 5.0f) { // Dejamos un margen pequeño para que no tiemble
        dirX = dx / distancia; // Normalizamos
        dirY = dy / distancia; // Normalizamos
    }

    // Le mandamos la orden a la arena 
    arena->comandoMoverJugador2(dirX, dirY, deltaTime);
}