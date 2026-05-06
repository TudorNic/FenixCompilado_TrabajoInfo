#include "ControladorIA.h"
#include <cmath>
#include <cstdlib>

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
        // Temporizador para el zig-zag amplio
        static float tiempoStrafe = 0.0f;
        static float tiempoParaCambiar = 2.0f;
        static float direccionStrafe = 1.0f;
        static float timerBrusco = 0.0f;

        tiempoStrafe += deltaTime;
        if (tiempoStrafe > tiempoParaCambiar) {
            direccionStrafe *= -1.0f;
            tiempoStrafe = 0.0f;
            tiempoParaCambiar = 2.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 2.0f));
            //35 % de probabilidad de hacer un quiebro BRUSCO al cambiar de dirección
            if (rand() % 100 < 35) {
                timerBrusco = 0.35f; // El efecto explosivo durará un tercio de segundo
            }
        }

        // Cálculo de dirección hacia el rival
        float dx = hHumano.x - hBot.x;
        float dy = hHumano.y - hBot.y;
        float distancia = std::sqrt(dx * dx + dy * dy);

        if (distancia > 0) {
            float normalX = dx / distancia;
            float normalY = dy / distancia;

            float ladoX = -normalY * direccionStrafe;
            float ladoY = normalX * direccionStrafe;

            float dirObjetivoX = 0.0f;
            float dirObjetivoY = 0.0f;
            float multiplicador = 1.0f;

            // Decidimos hacia dónde quiere ir
            if (distancia > 350.0f) {
                dirObjetivoX = normalX * 0.8f + ladoX * 0.2f;
                dirObjetivoY = normalY * 0.8f + ladoY * 0.2f;
            }
            else if (distancia < 200.0f) {
                dirObjetivoX = -normalX * 0.7f + ladoX * 0.4f;
                dirObjetivoY = -normalY * 0.7f + ladoY * 0.4f;
            }
            else {
                dirObjetivoX = ladoX * 0.9f - normalX * 0.1f;
                dirObjetivoY = ladoY * 0.9f - normalY * 0.1f;
                multiplicador = 0.75f;
            }

            // Normalizamos el objetivo
            float len = std::sqrt(dirObjetivoX * dirObjetivoX + dirObjetivoY * dirObjetivoY);
            if (len > 0) {
                dirObjetivoX /= len;
                dirObjetivoY /= len;
            }

            dirObjetivoX *= multiplicador;
            dirObjetivoY *= multiplicador;

            // Guardamos la velocidad real que tiene el personaje en este momento
            static float velocidadActualX = 0.0f;
            static float velocidadActualY = 0.0f;

            // La aceleración define lo pesado que se siente el personaje
            float aceleracionActual = 8.5f;

            if (timerBrusco > 0.0f) {
                timerBrusco -= deltaTime;
                aceleracionActual = 40.0f;   // Reacciona al instante, sin resbalar nada
                multiplicador *= 1.6f;
            }

            // Interpolamos la velocidad actual hacia la velocidad objetivo
            velocidadActualX += (dirObjetivoX - velocidadActualX) * aceleracionActual * deltaTime;
            velocidadActualY += (dirObjetivoY - velocidadActualY) * aceleracionActual * deltaTime;

            dirX = velocidadActualX;
            dirY = velocidadActualY;
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
        if (distancia < 450.0f && bot->puedeAtacar()) {
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
        float velocidadReal = bot->getVelocidad() * 100.0f;
        float nuevaX = hBot.x + (dirX * velocidadReal * deltaTime);
        float nuevaY = hBot.y + (dirY * velocidadReal * deltaTime);

        float limiteAncho = 1280.0f;
        float limiteAlto = 720.0f;
        float margen = 25.0f;

        nuevaX = std::clamp(nuevaX, margen, limiteAncho - margen - hBot.ancho);
        nuevaY = std::clamp(nuevaY, margen, limiteAlto - margen - hBot.alto);

        bot->setPosicion(nuevaX, nuevaY);
    }
}