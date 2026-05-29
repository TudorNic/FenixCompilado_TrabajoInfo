#include "ControladorIA.h"
#include <cmath>
#include <cstdlib>
#include <algorithm>

bool ControladorIA::detectarPeligro(float& dirEscapeX, float& dirEscapeY) {
    const auto& listaProyectiles = arena->getProyectiles();
    Hitbox hBot = bot->getHitbox();

    for (const auto& p : listaProyectiles) {
        if (bot->getNombreClase() == "aficion") return false;

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

    dificultadActual = DificultadIA::NORMAL;

    temporizadorInicio = 0.5f;
}

void ControladorIA::actualizar(float deltaTime) {
    // No hacer nada si la partida ha acabado
    if (arena->isTerminado() || bot->estaMuerto() || humano->estaMuerto()) {
        return;
    }

    if (temporizadorInicio > 0.0f) {
        temporizadorInicio -= deltaTime;
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
            int probabilidadQuiebro = 0;
            if (dificultadActual == DificultadIA::NORMAL) probabilidadQuiebro = 15;
            else if (dificultadActual == DificultadIA::DIFICIL) probabilidadQuiebro = 35;
            // En Fácil se queda en 0, nunca hace quiebros bruscos
            if (rand() % 100 < 35) {
                timerBrusco = 0.35f;
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
            if (bot->getNombreClase() == "aficion") {
                if (dificultadActual == DificultadIA::FACIL) {
                    dirObjetivoX = normalX * 0.7f + ladoX * 0.3f;
                    dirObjetivoY = normalY * 0.7f + ladoY * 0.3f;
                    multiplicador = 0.35f;
                }
                else if (dificultadActual == DificultadIA::NORMAL) {
                    dirObjetivoX = normalX;
                    dirObjetivoY = normalY;
                    multiplicador = 0.65f;
                }
                else {
                    dirObjetivoX = normalX;
                    dirObjetivoY = normalY;
                    multiplicador = 0.9f;
                }
            }
            else {
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
                aceleracionActual = 40.0f;
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

        if (bot->getNombreClase() == "aficion") {
            if (distancia < 85.0f && bot->puedeAtacar()) {
                bool reacciona = true;

                if (dificultadActual == DificultadIA::FACIL) {
                    if (rand() % 100 < 60) reacciona = false;
                }
                else if (dificultadActual == DificultadIA::NORMAL) {
                    if (rand() % 100 < 20) reacciona = false;
                }
                if (reacciona) {
                    float velDisparoX = dx / distancia;
                    float velDisparoY = dy / distancia;
                    arena->comandoDisparoJugador2(velDisparoX, velDisparoY);
                    bot->reiniciarRecarga();
                    bot->activarAnimacionAtaque();
                }
            }
        }
        else {
            // Deducir velocidad del humano
            static float lastHumanoX = hHumano.x;
            static float lastHumanoY = hHumano.y;

            float velHumanoX = (hHumano.x - lastHumanoX) / deltaTime;
            float velHumanoY = (hHumano.y - lastHumanoY) / deltaTime;
            lastHumanoX = hHumano.x;
            lastHumanoY = hHumano.y;

            float dx = hHumano.x - hBot.x;
            float dy = hHumano.y - hBot.y;
            float distancia = std::sqrt(dx * dx + dy * dy);
            //USO DE HABILIDAD ESPECIAL
            if (bot->getNombreClase() == "entrenador" && bot->puedeUsarEspecial()) {
                if (bot->getVidaActual() <= 40) {
                    arena->comandoEspecialJugador2();
                    bot->reiniciarEspecial();
                }
            }
            // Si está a tiro y el arma se ha recargado
            if (distancia < 450.0f && bot->puedeAtacar()) {
                float velocidadProyectil = 420.0f;
                float tiempoImpacto = distancia / velocidadProyectil;

                float futuroX = hHumano.x;
                float futuroY = hHumano.y;

                // En nivel Fácil no predice el futuro, tira a donde estás
                if (dificultadActual != DificultadIA::FACIL) {
                    futuroX += (velHumanoX * tiempoImpacto);
                    futuroY += (velHumanoY * tiempoImpacto);
                }
                float errorApuntado = 0.0f;
                if (dificultadActual == DificultadIA::FACIL) errorApuntado = 120.0f;
                else if (dificultadActual == DificultadIA::NORMAL) errorApuntado = 40.0f;

                if (errorApuntado > 0.0f) {
                    // Desvío aleatorio
                    futuroX += ((rand() % 200) / 100.0f - 1.0f) * errorApuntado;
                    futuroY += ((rand() % 200) / 100.0f - 1.0f) * errorApuntado;
                }
                float predX = futuroX - hBot.x;
                float predY = futuroY - hBot.y;
                float distPred = std::sqrt(predX * predX + predY * predY);

                if (distPred > 0) {
                    float velDisparoX = predX / distPred;
                    float velDisparoY = predY / distPred;

                    arena->comandoDisparoJugador2(velDisparoX, velDisparoY);
                    bot->reiniciarRecarga();
                    bot->activarAnimacionAtaque();
                }
            }
        }
    }
        // Aplicar movimiento
        if (dirX != 0.0f || dirY != 0.0f) {
            float velocidadReal = bot->getVelocidad() * 100.0f;
            float nuevaX = hBot.x + (dirX * velocidadReal * deltaTime);
            float nuevaY = hBot.y + (dirY * velocidadReal * deltaTime);

            float limiteAncho = 600.0f;
            float limiteAlto = 560.0f;
            float margen = 25.0f;

            nuevaX = std::clamp(nuevaX, margen, limiteAncho - margen - hBot.ancho);
            nuevaY = std::clamp(nuevaY, margen, limiteAlto - margen - hBot.alto);

            bot->setPosicion(nuevaX, nuevaY);

            bot->setEstado(CAMINANDO);
        }
    else {
        bot->setEstado(QUIETO);
    }
}