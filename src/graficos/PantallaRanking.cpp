#include "PantallaRanking.h"
#include <stdexcept>

PantallaRanking::PantallaRanking(sf::RenderWindow& v, const std::string& rutaFuente)
    : ventana(v),
    fuente(),
    titulo(fuente),
    subtitulo(fuente),
    rankingTexto(fuente),
    textoVolver(fuente),
    volverAlMenu(false)
{
    if (!fuente.openFromFile(rutaFuente))
    {
        throw std::runtime_error("No se pudo cargar la fuente del ranking.");
    }

    inicializarTextos();
}

void PantallaRanking::procesarEventos()
{
    while (const auto evento = ventana.pollEvent())
    {
        if (evento->is<sf::Event::Closed>())
        {
            ventana.close();
        }

        if (const auto* tecla = evento->getIf<sf::Event::KeyPressed>())
        {
            if (tecla->code == sf::Keyboard::Key::Escape)
            {
                volverAlMenu = true;
            }
        }
    }
}

void PantallaRanking::actualizar()
{
}

void PantallaRanking::dibujar()
{
    ventana.clear(sf::Color(20, 20, 20));

    ventana.draw(titulo);
    ventana.draw(subtitulo);
    ventana.draw(rankingTexto);
    ventana.draw(textoVolver);

    ventana.display();
}

bool PantallaRanking::debeVolverAlMenu() const
{
    return volverAlMenu;
}

void PantallaRanking::reiniciarVolver()
{
    volverAlMenu = false;
}

void PantallaRanking::inicializarTextos()
{
    titulo.setString("RANKING");
    titulo.setCharacterSize(34);
    titulo.setFillColor(sf::Color::White);
    titulo.setPosition({ 260.f, 40.f });

    subtitulo.setString("Clasificacion provisional");
    subtitulo.setCharacterSize(20);
    subtitulo.setFillColor(sf::Color::Yellow);
    subtitulo.setPosition({ 220.f, 95.f });

    rankingTexto.setString(
        "POS   JUGADOR         PUNTOS   VICTORIAS   DERROTAS\n\n"
        "1     Sergio          1200     8           2\n"
        "2     Pablo           980      6           3\n"
        "3     IA              760      4           5\n"
        "4     Invitado        540      3           6\n"
    );
    rankingTexto.setCharacterSize(18);
    rankingTexto.setFillColor(sf::Color(220, 220, 220));
    rankingTexto.setPosition({ 90.f, 180.f });

    textoVolver.setString("Pulsa ESC para volver al menu");
    textoVolver.setCharacterSize(18);
    textoVolver.setFillColor(sf::Color::Yellow);
    textoVolver.setPosition({ 220.f, 520.f });
}
