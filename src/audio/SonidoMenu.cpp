#include "SonidoMenu.h"
#include <stdexcept>

SonidoMenu::SonidoMenu(const std::string& rutaMover,
    const std::string& rutaConfirmar,
    const std::string& rutaSalir,
    const std::string& rutaMusica) 
	: bufferMover(), // Inicialización de los buffers de sonido ya que sf:Sound no tiene constructor por defecto en SFML 3
    bufferConfirmar(),
    bufferSalir(),
    sonidoMover(bufferMover),
    sonidoConfirmar(bufferConfirmar),
    sonidoSalir(bufferSalir),
    musicaFondo() 
{
    if (!bufferMover.loadFromFile(rutaMover))
    {
        throw std::runtime_error("No se pudo cargar el sonido de mover opcion.");
    }

    if (!bufferConfirmar.loadFromFile(rutaConfirmar))
    {
        throw std::runtime_error("No se pudo cargar el sonido de confirmar opcion.");
    }

    if (!bufferSalir.loadFromFile(rutaSalir))
    {
        throw std::runtime_error("No se pudo cargar el sonido de salir del menu.");
    }

    if (!musicaFondo.openFromFile(rutaMusica))
    {
        throw std::runtime_error("No se pudo cargar la musica de fondo del menu.");
    }

    sonidoMover.setBuffer(bufferMover);
    sonidoConfirmar.setBuffer(bufferConfirmar);
    sonidoSalir.setBuffer(bufferSalir);

    musicaFondo.setLooping(true);

    setVolumenEfectos(30.f);
    setVolumenMusica(50.f);
}

void SonidoMenu::reproducirMover()
{
    sonidoMover.play();
}

void SonidoMenu::reproducirConfirmar()
{
    sonidoConfirmar.play();
}

void SonidoMenu::reproducirSalir()
{
    sonidoSalir.play();
}

void SonidoMenu::reproducirMusica()
{
    musicaFondo.play();
}

void SonidoMenu::detenerMusica()
{
    musicaFondo.stop();
}

void SonidoMenu::setVolumenEfectos(float volumen)
{
    sonidoMover.setVolume(volumen);
    sonidoConfirmar.setVolume(volumen);
    sonidoSalir.setVolume(volumen);
}

void SonidoMenu::setVolumenMusica(float volumen)
{
    musicaFondo.setVolume(volumen);
}