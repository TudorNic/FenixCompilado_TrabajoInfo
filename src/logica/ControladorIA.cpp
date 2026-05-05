#include "ControladorIA.h"
#include <cmath>

bool ControladorIA::detectarPeligro(float& dirEscapeX, float& dirEscapeY) {
    const auto& listaProyectiles = arena->getProyectiles();
    Hitbox hBot = bot->getHitbox();

    for (const auto& p : listaProyectiles) {
        // Ignorar proyectiles que ya han chocado
        if (p.isActivo()) {
            Hitbox hBalon = p.getHitbox();

            
            float dx = hBot.x - hBalon.x;
            float dy = hBot.y - hBalon.y;
            float distancia = std::sqrt(dx * dx + dy * dy);

            // Si el balón está a menos de 100 píxeles, se activa la huida
            if (distancia < 100.0f) {
                dirEscapeX = dx / distancia;
                dirEscapeY = dy / distancia;
                return true;
            }
        }
    }
    return false;
}

ControladorIA::ControladorIA(Jugador* botPtr, Jugador* humanoPtr, Arena* arenaPtr) {
    bot = botPtr;
    humano = humanoPtr;
    arena = arenaPtr;
    estadoActual = EstadoIA::PERSEGUIR;
}

void ControladorIA::actualizar(float deltaTime) {
    // No hacer nada si la partida ha acabado
    if (arena->isTerminado() || bot->estaMuerto() || humano->estaMuerto()) {
        return;
    }

    Hitbox hBot = bot->getHitbox();
    Hitbox hHumano = humano->getHitbox();

    float dirX = 0.0f;
    float dirY = 0.0f;

    // Decidir qué hacer según el entorno
    float escapeX = 0.0f, escapeY = 0.0f;
    if (detectarPeligro(escapeX, escapeY)) {
        estadoActual = EstadoIA::ESQUIVAR;
    }
    else {
        estadoActual = EstadoIA::PERSEGUIR;
    }

    // Ejecutar comportamiento de la FSM
    switch (estadoActual) {
    case EstadoIA::ESQUIVAR: {
        dirX = escapeX;
        dirY = escapeY;
        break;
    }

    case EstadoIA::PERSEGUIR: {
        // Cálculo de dirección hacia el rival
        float dx = hHumano.x - hBot.x;
        float dy = hHumano.y - hBot.y;
        float distancia = std::sqrt(dx * dx + dy * dy);

        // Solo se acerca si no está en rango de colisión
        if (distancia > 5.0f) {
            dirX = dx / distancia;
            dirY = dy / distancia;
        }
        break;
    }

    }
    //Disparar si se está cerca
    if (estadoActual == EstadoIA::PERSEGUIR) {
        float dx = hHumano.x - hBot.x;
        float dy = hHumano.y - hBot.y;
        float distancia = std::sqrt(dx * dx + dy * dy);

        // Si está a tiro y el arma se ha recargado
        if (distancia < 300.0f && bot->puedeAtacar()) {
            float velDisparoX = dx / distancia;
            float velDisparoY = dy / distancia;
            //Disparo
            arena->comandoDisparoJugador2(velDisparoX, velDisparoY);
            //Recarga
            bot->reiniciarRecarga();
        }
    }
    // Aplicar movimiento final
    if (dirX != 0.0f || dirY != 0.0f) {
        float velocidadReal = bot->getVelocidad() * 50.0f;
        float nuevaX = hBot.x + (dirX * velocidadReal * deltaTime);
        float nuevaY = hBot.y + (dirY * velocidadReal * deltaTime);

        bot->setPosicion(nuevaX, nuevaY);
    }
}