#pragma once

#include <string>
#include <vector>

struct RegistroJugador
{
    std::string nombre;
    int victorias;
    int derrotas;
    int partidasJugadas;

    float obtenerPromedio() const;
};

class Ranking
{
private:
    std::vector<RegistroJugador> jugadores;
    std::string rutaFichero;

public:
    Ranking(const std::string& ruta);

    void cargar();
    void guardar() const;

    void registrarResultado(const std::string& nombreJugador, bool victoria);

    const std::vector<RegistroJugador>& obtenerJugadores() const;

    void ordenarRanking();

private:
    int buscarJugador(const std::string& nombreJugador) const;
};

