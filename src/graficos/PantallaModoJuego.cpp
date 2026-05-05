#include "PantallaModoJuego.h"
#include <stdexcept>

PantallaModoJuego::PantallaModoJuego(sf::RenderWindow& v, const std::string& rutaFuente)
    : ventana(v),
    fuente(),
    titulo(fuente),
    opcionesTexto{ sf::Text(fuente), sf::Text(fuente) },
    textoVolver(fuente),
    opcionSeleccionada(0),
    opcionConfirmada(false),
    volverAlMenu(false)
{
    if (!fuente.openFromFile(rutaFuente))
    {
        throw std::runtime_error("No se pudo cargar la fuente del modo de juego.");
    }

    inicializarTextos();
    actualizarAspectoOpciones();
}

void PantallaModoJuego::procesarEventos()
{
    while (const auto evento = ventana.pollEvent())
    {
        if (evento->is<sf::Event::Closed>())
        {
            ventana.close();
        }

        if (const auto* tecla = evento->getIf<sf::Event::KeyPressed>())
        {
            if (tecla->code == sf::Keyboard::Key::Up || tecla->code == sf::Keyboard::Key::W)
            {
                moverArriba();
            }
            else if (tecla->code == sf::Keyboard::Key::Down || tecla->code == sf::Keyboard::Key::S)
            {
                moverAbajo();
            }
            else if (tecla->code == sf::Keyboard::Key::Enter)
            {
                opcionConfirmada = true;
            }
            else if (tecla->code == sf::Keyboard::Key::Escape)
            {
                volverAlMenu = true;
            }
        }
    }
}

void PantallaModoJuego::actualizar()
{
    actualizarAspectoOpciones();
}

void PantallaModoJuego::dibujar()
{
    ventana.clear(sf::Color(20, 20, 20));

    ventana.draw(titulo);
    ventana.draw(opcionesTexto[0]);
    ventana.draw(opcionesTexto[1]);
    ventana.draw(textoVolver);

    ventana.display();
}

bool PantallaModoJuego::estaOpcionConfirmada() const
{
    return opcionConfirmada;
}

PantallaModoJuego::OpcionModo PantallaModoJuego::obtenerOpcionConfirmada() const
{
    return static_cast<OpcionModo>(opcionSeleccionada);
}

void PantallaModoJuego::reiniciarConfirmacion()
{
    opcionConfirmada = false;
}

bool PantallaModoJuego::debeVolverAlMenu() const
{
    return volverAlMenu;
}

void PantallaModoJuego::reiniciarVolver()
{
    volverAlMenu = false;
}

void PantallaModoJuego::inicializarTextos()
{
    titulo.setString("SELECCIONA EL MODO DE JUEGO");
    titulo.setCharacterSize(28);
    titulo.setFillColor(sf::Color::White);
    titulo.setPosition({ 120.f, 80.f });

    opcionesTexto[0].setString("Jugador vs IA");
    opcionesTexto[0].setCharacterSize(30);
    opcionesTexto[0].setPosition({ 250.f, 220.f });

    opcionesTexto[1].setString("Jugador vs Jugador");
    opcionesTexto[1].setCharacterSize(30);
    opcionesTexto[1].setPosition({ 210.f, 300.f });

    textoVolver.setString("Pulsa ESC para volver al menu");
    textoVolver.setCharacterSize(18);
    textoVolver.setFillColor(sf::Color::Yellow);
    textoVolver.setPosition({ 220.f, 520.f });
}

void PantallaModoJuego::actualizarAspectoOpciones()
{
    for (int i = 0; i < 2; i++)
    {
        if (i == opcionSeleccionada)
        {
            opcionesTexto[i].setFillColor(sf::Color::Yellow);
            opcionesTexto[i].setScale({ 1.12f, 1.12f });
        }
        else
        {
            opcionesTexto[i].setFillColor(sf::Color(220, 220, 220));
            opcionesTexto[i].setScale({ 1.f, 1.f });
        }
    }
}

void PantallaModoJuego::moverArriba()
{
    if (opcionSeleccionada > 0)
    {
        opcionSeleccionada--;
    }
}

void PantallaModoJuego::moverAbajo()
{
    if (opcionSeleccionada < 1)
    {
        opcionSeleccionada++;
    }
}