#pragma once

#include <SFML/Audio.hpp>
#include <string>

class SonidoJuego
{
private:
    sf::Music ambienteTablero;

    sf::SoundBuffer bufferEntradaCombate;
    sf::Sound sonidoEntradaCombate;

public:
    SonidoJuego(const std::string& rutaAmbienteTablero,
        const std::string& rutaEntradaCombate);

    void reproducirAmbienteTablero();
    void detenerAmbienteTablero();
    void pausarAmbienteTablero();

    void reproducirEntradaCombate();

    void setVolumenAmbiente(float volumen);
    void setVolumenEfectoCombate(float volumen);
};
