#include "PantallaRanking.h"
#include <stdexcept>
#include <sstream> //Para construir textos como si estuvieramos escribiendo un flujo
#include <iomanip> //PAra dar formato a textos y números

static void centrarTextoRanking(sf::Text& texto, float y)
{
    sf::FloatRect bounds = texto.getLocalBounds();
    texto.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
    texto.setPosition(500.f, y);
}

PantallaRanking::PantallaRanking(sf::RenderWindow& v, Ranking& r, const std::string& rutaFuente)
    : ventana(v),
    ranking(r),
    volverAlMenu(false)
{
    if (!fuente.loadFromFile(rutaFuente))
    {
        throw std::runtime_error("No se pudo cargar la fuente del ranking.");
    }

    titulo.setFont(fuente);
    subtitulo.setFont(fuente);
    rankingTexto.setFont(fuente);
    textoVolver.setFont(fuente);

    inicializarTextos();
	actualizarTextoRanking();
}

void PantallaRanking::procesarEventos()
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
            if (evento.key.code == sf::Keyboard::Escape)
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
    titulo.setCharacterSize(42);
    titulo.setFillColor(sf::Color::White);
    centrarTextoRanking(titulo, 100.f);

    subtitulo.setString("Clasificacion general");
    subtitulo.setCharacterSize(22);
    subtitulo.setFillColor(sf::Color::Yellow);
    centrarTextoRanking(subtitulo, 160.f);

    rankingTexto.setCharacterSize(22);
    rankingTexto.setFillColor(sf::Color(220, 220, 220));
    rankingTexto.setPosition({ 190.f, 260.f });

    textoVolver.setString("Pulsa ESC para volver al menu");
    textoVolver.setCharacterSize(18);
    textoVolver.setFillColor(sf::Color::Yellow);
    centrarTextoRanking(textoVolver, 900.f);
}

void PantallaRanking::actualizarTextoRanking()
{
    std::ostringstream ss;

    const std::vector<RegistroJugador>& jugadores = ranking.obtenerJugadores();

    if (jugadores.empty()) {
        rankingTexto.setString("Todavia no hay partidas registradas.");
        return;
    }

    ss << "=== TOP JUGADOR VS JUGADOR (PvP) ===\n";

    ss  << std::left << std::setw(4) << "POS"
        << std::setw(14) << " JUGADOR"
        << std::setw(8) << "V"
        << std::setw(8) << "D"
        << std::setw(8) << "PJ"
        << "PROM\n\n";

    int contPvP = 0;

    for (size_t i = 0; i < jugadores.size(); i++)
    {
        std::string nom = jugadores[i].nombre;

        // Filtro: Si el nombre contiene "IA", "ia" o "Ia", es la maquina y no se imprime
        bool esMaquina = (nom.find("IA") != std::string::npos ||
            nom.find("ia") != std::string::npos ||
            nom.find("Ia") != std::string::npos);
        if (!esMaquina) {
            ss << std::left
                << std::setw(9) << (std::to_string(i + 1) + ".")
                << std::setw(18) << jugadores[i].nombre
                << std::setw(9) << jugadores[i].victorias
                << std::setw(9) << jugadores[i].derrotas
                << std::setw(9) << jugadores[i].partidasJugadas
                << std::fixed << std::setprecision(2) << jugadores[i].obtenerPromedio()
                << "\n";
            contPvP++;
        }
        if (contPvP >= 2) break;
    }
    if (contPvP == 0) ss << "No hay registros PvP todavía.\n";
    ss << "\n--------------------------------------------------\n\n";

    ss << "=== TOP JUGADOR VS IA (PvE) ===\n";
    ss << std::left << std::setw(5) << "POS"
        << std::setw(14) << "JUGADOR"
        << std::setw(8) << "V"
        << std::setw(8) << "D"
        << std::setw(8) << "PJ"
        << "PROM\n\n";

    int contIA = 0;
    for (size_t i = 0; i < jugadores.size(); i++) {
        std::string nom = jugadores[i].nombre;
            bool esMaquina = (nom.find("IA") != std::string::npos ||
                nom.find("ia") != std::string::npos ||
                nom.find("Ia") != std::string::npos);
        if (esMaquina) {
            ss << std::left
                << std::setw(5) << (std::to_string(contIA + 1) + ".")
                << std::setw(14) << jugadores[i].nombre
                << std::setw(8) << jugadores[i].victorias
                << std::setw(8) << jugadores[i].derrotas
                << std::setw(8) << jugadores[i].partidasJugadas
                << std::fixed << std::setprecision(2) << jugadores[i].obtenerPromedio()
                << "\n";
            contIA++;
        }
        if (contIA >= 2) break; 
    }
    if (contIA == 0) ss << "No hay registros contra la IA todavia.\n";






    if (jugadores.empty())
    {
        ss << "Todavia no hay partidas registradas.";
    }

    rankingTexto.setString(ss.str());
}