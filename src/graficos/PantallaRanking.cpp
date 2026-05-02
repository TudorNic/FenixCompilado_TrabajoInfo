#include "PantallaRanking.h"
#include <stdexcept>
#include <sstream> //Para construir textos como si estuvieramos escribiendo un flujo
#include <iomanip> //PAra dar formato a textos y números

PantallaRanking::PantallaRanking(sf::RenderWindow& v, Ranking& r, const std::string& rutaFuente)
    : ventana(v),
    ranking(r),
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
	actualizarTextoRanking();
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
    actualizarTextoRanking();
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

    subtitulo.setString("Clasificacion general");
    subtitulo.setCharacterSize(20);
    subtitulo.setFillColor(sf::Color::Yellow);
    subtitulo.setPosition({ 220.f, 95.f });

    rankingTexto.setCharacterSize(18);
    rankingTexto.setFillColor(sf::Color(220, 220, 220));
    rankingTexto.setPosition({ 70.f, 170.f });

    textoVolver.setString("Pulsa ESC para volver al menu");
    textoVolver.setCharacterSize(18);
    textoVolver.setFillColor(sf::Color::Yellow);
    textoVolver.setPosition({ 220.f, 520.f });
}

void PantallaRanking::actualizarTextoRanking()
{
    std::ostringstream ss;

    ss << "POS  "
        << std::left << std::setw(14) << "JUGADOR"
        << std::setw(8) << "V"
        << std::setw(8) << "D"
        << std::setw(8) << "PJ"
        << "PROM\n\n";

    const std::vector<RegistroJugador>& jugadores = ranking.obtenerJugadores();

    for (size_t i = 0; i < jugadores.size(); i++)
    {
        ss << std::left
            << std::setw(5) << (std::to_string(i + 1) + ".")
            << std::setw(14) << jugadores[i].nombre
            << std::setw(8) << jugadores[i].victorias
            << std::setw(8) << jugadores[i].derrotas
            << std::setw(8) << jugadores[i].partidasJugadas
            << std::fixed << std::setprecision(2) << jugadores[i].obtenerPromedio()
            << "\n";
    }

    if (jugadores.empty())
    {
        ss << "Todavia no hay partidas registradas.";
    }

    rankingTexto.setString(ss.str());
}