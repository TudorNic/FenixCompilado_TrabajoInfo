#include "PantallaDificultadIA.h"
#include <stdexcept>

static void centrarTextoDificultad(sf::Text& texto, float y)
{
    sf::FloatRect bounds = texto.getLocalBounds();
    texto.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
    texto.setPosition(500.f, y);
}

PantallaDificultadIA::PantallaDificultadIA(sf::RenderWindow& v,
    const std::string& rutaFuente,
    const std::string& rutaFondo,
    const std::string& rutaMover)
    : ventana(v),
    opcionSeleccionada(0),
    opcionConfirmada(false),
    volverAlModoJuego(false)
{
    if (!fuente.loadFromFile(rutaFuente))
    {
        throw std::runtime_error("No se pudo cargar la fuente de dificultad IA.");
    }

    if (!texturaFondo.loadFromFile(rutaFondo))
    {
        throw std::runtime_error("No se pudo cargar el fondo de dificultad IA.");
    }

    if (!bufferMover.loadFromFile(rutaMover))
    {
        throw std::runtime_error("No se pudo cargar el sonido de mover opcion en dificultad IA.");
    }

    sonidoMover.setBuffer(bufferMover);
    sonidoMover.setVolume(35.f);

    titulo.setFont(fuente);
    opcionesTexto[0].setFont(fuente);
    opcionesTexto[1].setFont(fuente);
    opcionesTexto[2].setFont(fuente);
    textoVolver.setFont(fuente);

    fondo.setTexture(texturaFondo, true);

    sf::Vector2u sizeVentana = ventana.getSize();
    sf::Vector2u sizeImagen = texturaFondo.getSize();

    fondo.setScale(
        static_cast<float>(sizeVentana.x) / static_cast<float>(sizeImagen.x),
        static_cast<float>(sizeVentana.y) / static_cast<float>(sizeImagen.y)
    );

    inicializarTextos();
    actualizarAspectoOpciones();
}

void PantallaDificultadIA::procesarEventos()
{
    sf::Event evento;

    while (ventana.pollEvent(evento))
    {
        if (evento.type == sf::Event::Closed)
        {
            ventana.close();
        }

        if (evento.type == sf::Event::KeyPressed)
        {
            if (evento.key.code == sf::Keyboard::Up || evento.key.code == sf::Keyboard::W)
            {
                moverArriba();
                sonidoMover.play();
            }
            else if (evento.key.code == sf::Keyboard::Down || evento.key.code == sf::Keyboard::S)
            {
                moverAbajo();
                sonidoMover.play();
            }
            else if (evento.key.code == sf::Keyboard::Return)
            {
                opcionConfirmada = true;
            }
            else if (evento.key.code == sf::Keyboard::Escape)
            {
                volverAlModoJuego = true;
            }
        }
    }
}

void PantallaDificultadIA::actualizar()
{
    actualizarAspectoOpciones();
}

void PantallaDificultadIA::dibujar()
{
    ventana.clear();

    ventana.draw(fondo);
    ventana.draw(titulo);
    ventana.draw(opcionesTexto[0]);
    ventana.draw(opcionesTexto[1]);
    ventana.draw(opcionesTexto[2]);
    ventana.draw(textoVolver);

    ventana.display();
}

bool PantallaDificultadIA::estaOpcionConfirmada() const
{
    return opcionConfirmada;
}

PantallaDificultadIA::Dificultad PantallaDificultadIA::obtenerDificultadConfirmada() const
{
    return static_cast<Dificultad>(opcionSeleccionada);
}

void PantallaDificultadIA::reiniciarConfirmacion()
{
    opcionConfirmada = false;
}

bool PantallaDificultadIA::debeVolverAlModoJuego() const
{
    return volverAlModoJuego;
}

void PantallaDificultadIA::reiniciarVolver()
{
    volverAlModoJuego = false;
}

void PantallaDificultadIA::inicializarTextos()
{
    titulo.setString("SELECCIONA DIFICULTAD");
    titulo.setCharacterSize(36);
    titulo.setFillColor(sf::Color::White);
    centrarTextoDificultad(titulo, 180.f);

    opcionesTexto[0].setString("Facil");
    opcionesTexto[0].setCharacterSize(34);
    centrarTextoDificultad(opcionesTexto[0], 340.f);

    opcionesTexto[1].setString("Normal");
    opcionesTexto[1].setCharacterSize(34);
    centrarTextoDificultad(opcionesTexto[1], 430.f);

    opcionesTexto[2].setString("Dificil");
    opcionesTexto[2].setCharacterSize(34);
    centrarTextoDificultad(opcionesTexto[2], 520.f);

    textoVolver.setString("Pulsa ESC para volver");
    textoVolver.setCharacterSize(20);
    textoVolver.setFillColor(sf::Color::Yellow);
    centrarTextoDificultad(textoVolver, 850.f);
}

void PantallaDificultadIA::actualizarAspectoOpciones()
{
    for (int i = 0; i < 3; i++)
    {
        if (i == opcionSeleccionada)
        {
            opcionesTexto[i].setFillColor(sf::Color::Yellow);
            opcionesTexto[i].setScale(1.12f, 1.12f);
        }
        else
        {
            opcionesTexto[i].setFillColor(sf::Color::Black);
            opcionesTexto[i].setScale(1.f, 1.f);
        }
    }

    centrarTextoDificultad(opcionesTexto[0], 340.f);
    centrarTextoDificultad(opcionesTexto[1], 430.f);
    centrarTextoDificultad(opcionesTexto[2], 520.f);
}

void PantallaDificultadIA::moverArriba()
{
    if (opcionSeleccionada > 0)
    {
        opcionSeleccionada--;
    }
}

void PantallaDificultadIA::moverAbajo()
{
    if (opcionSeleccionada < 2)
    {
        opcionSeleccionada++;
    }
}