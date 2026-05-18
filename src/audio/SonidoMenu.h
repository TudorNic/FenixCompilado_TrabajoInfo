#pragma once

#include <SFML/Audio.hpp>
#include <string>

class SonidoMenu
{
private:
    sf::SoundBuffer bufferMover;
    sf::SoundBuffer bufferConfirmar;
    sf::SoundBuffer bufferSalir;

    sf::Sound sonidoMover;
    sf::Sound sonidoConfirmar;
    sf::Sound sonidoSalir;

    sf::Music musicaFondo;

public:
    SonidoMenu(const std::string& rutaMover,
        const std::string& rutaConfirmar,
        const std::string& rutaSalir,
        const std::string& rutaMusica);

    void reproducirMover();
    void reproducirConfirmar();
    void reproducirSalir();

    void reproducirMusica();
    void detenerMusica();

    void setVolumenEfectos(float volumen);
    void setVolumenMusica(float volumen);
};

