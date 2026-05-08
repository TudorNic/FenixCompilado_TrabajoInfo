#include <iostream>
#include "tablero.h"
#include "Pieza.h"

int main() {
    std::cout << "--- Iniciando ArchonFutbol: Edicion Lord Supremo ---" << std::endl;

    // 1. Intentamos crear el tablero
    Tablero miCampo;
    std::cout << "Tablero 9x9 generado correctamente." << std::endl;
    Tablero miTablero;
    miTablero.Inicializar_Partida();
    miTablero.mostrarTableroprueba();
    miTablero.seleccionar_Pieza(0, 6);
    miTablero.mover_Pieza(1, 6);
    miTablero.mostrarTableroprueba();
    miTablero.seleccionar_Pieza(8, 3);
    miTablero.mover_Pieza(7, 3);
    miTablero.mostrarTableroprueba();
  


    std::cout << "Partido listo para comenzar." << std::endl;

    return 0;
}