#include "PantallaModoJuego.h"
#include <stdexcept>

void centrarTextoModo(sf::Text& texto, float y)
{
    sf::FloatRect bounds = texto.getLocalBounds();
    texto.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
    texto.setPosition(500.f, y);
}

PantallaModoJuego::PantallaModoJuego(sf::RenderWindow& v, const std::string& rutaFuente, const std::string& rutaFondo)
    : ventana(v),
    opcionSeleccionada(0),
    opcionConfirmada(false),
    volverAlMenu(false)
{
    if (!fuente.loadFromFile(rutaFuente))
    {
        throw std::runtime_error("No se pudo cargar la fuente del modo de juego.");
    }

    if (!texturaFondo.loadFromFile(rutaFondo))
    {
        throw std::runtime_error("No se pudo cargar el fondo del modo de juego.");
    }

	titulo.setFont(fuente);
    opcionesTexto[0].setFont(fuente);
    opcionesTexto[1].setFont(fuente);
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

void PantallaModoJuego::procesarEventos()
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
            }
            else if (evento.key.code == sf::Keyboard::Down || evento.key.code == sf::Keyboard::S)
            {
                moverAbajo();
            }
            else if (evento.key.code == sf::Keyboard::Return)
            {
                opcionConfirmada = true;
            }
            else if (evento.key.code == sf::Keyboard::Escape)
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
    ventana.clear();

    ventana.draw(fondo);

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
    centrarTextoModo(titulo, 180.f);

    opcionesTexto[0].setString("Jugador vs IA");
    opcionesTexto[0].setCharacterSize(30);
    opcionesTexto[0].setPosition({ 250.f, 220.f });
    centrarTextoModo(opcionesTexto[0], 340.f);


    opcionesTexto[1].setString("Jugador vs Jugador");
    opcionesTexto[1].setCharacterSize(30);
    opcionesTexto[1].setPosition({ 210.f, 300.f });
    centrarTextoModo(opcionesTexto[1], 430.f);

    textoVolver.setString("Pulsa ESC para volver al menu");
    textoVolver.setCharacterSize(18);
    textoVolver.setFillColor(sf::Color::Yellow);
    textoVolver.setPosition({ 220.f, 520.f });
    centrarTextoModo(textoVolver, 850.f);

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
    centrarTextoModo(opcionesTexto[0], 340.f);
    centrarTextoModo(opcionesTexto[1], 430.f);
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