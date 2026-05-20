#include "SonidoJuego.h"
#include <stdexcept>

SonidoJuego::SonidoJuego(const std::string& rutaAmbienteTablero,
    const std::string& rutaEntradaCombate)
{
    if (!ambienteTablero.openFromFile(rutaAmbienteTablero))
    {
        throw std::runtime_error("No se pudo cargar el ambiente del tablero.");
    }

    if (!bufferEntradaCombate.loadFromFile(rutaEntradaCombate))
    {
        throw std::runtime_error("No se pudo cargar el sonido de entrada al combate.");
    }

    sonidoEntradaCombate.setBuffer(bufferEntradaCombate);

    ambienteTablero.setLoop(true);

    setVolumenAmbiente(35.f);
    setVolumenEfectoCombate(75.f);
}

void SonidoJuego::reproducirAmbienteTablero()
{
    if (ambienteTablero.getStatus() != sf::Music::Playing)
    {
        ambienteTablero.play();
    }
}

void SonidoJuego::detenerAmbienteTablero()
{
    ambienteTablero.stop();
}

void SonidoJuego::pausarAmbienteTablero()
{
    ambienteTablero.pause();
}

void SonidoJuego::reproducirEntradaCombate()
{
    sonidoEntradaCombate.play();
}

void SonidoJuego::setVolumenAmbiente(float volumen)
{
    ambienteTablero.setVolume(volumen);
}

void SonidoJuego::setVolumenEfectoCombate(float volumen)
{
    sonidoEntradaCombate.setVolume(volumen);
}
