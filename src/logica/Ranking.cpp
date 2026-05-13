#include "Ranking.h"
#include <fstream>
#include <algorithm>

float RegistroJugador::obtenerPromedio() const
{
    if (partidasJugadas == 0)
    {
        return 0.0f;
    }

    return static_cast<float>(victorias) / static_cast<float>(partidasJugadas);
}

Ranking::Ranking(const std::string& ruta)
    : rutaFichero(ruta)
{
}

void Ranking::cargar()
{
    jugadores.clear();

    std::ifstream fichero(rutaFichero);

    if (!fichero.is_open())
    {
        return;
    }

    RegistroJugador jugador;

    while (fichero >> jugador.nombre >> jugador.victorias >> jugador.derrotas >> jugador.partidasJugadas)
    {
        jugadores.push_back(jugador);
    }

    fichero.close();
    ordenarRanking();
}

void Ranking::guardar() const
{
    std::ofstream fichero(rutaFichero);

    if (!fichero.is_open())
    {
        return;
    }

    for (const auto& jugador : jugadores)
    {
        fichero << jugador.nombre << " "
            << jugador.victorias << " "
            << jugador.derrotas << " "
            << jugador.partidasJugadas << "\n";
    }

    fichero.close();
}

void Ranking::registrarResultado(const std::string& nombreJugador, bool victoria)
{
    int indice = buscarJugador(nombreJugador);

    if (indice == -1)
    {
        RegistroJugador nuevoJugador;
        nuevoJugador.nombre = nombreJugador;
        nuevoJugador.victorias = 0;
        nuevoJugador.derrotas = 0;
        nuevoJugador.partidasJugadas = 0;

        jugadores.push_back(nuevoJugador);
        indice = static_cast<int>(jugadores.size()) - 1;
    }

    jugadores[indice].partidasJugadas++;

    if (victoria)
    {
        jugadores[indice].victorias++;
    }
    else
    {
        jugadores[indice].derrotas++;
    }

    ordenarRanking();
}

const std::vector<RegistroJugador>& Ranking::obtenerJugadores() const
{
    return jugadores;
}

void Ranking::ordenarRanking()
{
    std::sort(jugadores.begin(), jugadores.end(),
        [](const RegistroJugador& a, const RegistroJugador& b)
        {
            if (a.obtenerPromedio() != b.obtenerPromedio())
            {
                return a.obtenerPromedio() > b.obtenerPromedio();
            }

            if (a.victorias != b.victorias)
            {
                return a.victorias > b.victorias;
            }

            return a.derrotas < b.derrotas;
        });
}

int Ranking::buscarJugador(const std::string& nombreJugador) const
{
    for (int i = 0; i < static_cast<int>(jugadores.size()); i++)
    {
        if (jugadores[i].nombre == nombreJugador)
        {
            return i;
        }
    }

    return -1;
}