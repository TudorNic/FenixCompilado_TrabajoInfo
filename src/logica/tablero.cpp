#include "tablero.h"
#include "Portero.h"
#include "Defensa.h"
#include "Delantero.h"
#include "Laterales.h"
#include "Centrocampistas.h"
#include "Pieza.h"
#include <iostream>

Tablero::Tablero() : Turno_Actual(1), Fase_Ciclo(0) {
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			casillas[i][j] = nullptr;
		}
	}
	Inicializar_Campo();
}

void Tablero::Inicializar_Campo() {
	posicion[0] = { 4,4 }; //Punto de poder centro del campo
	posicion[1] = { 4,0 }; //Punto de poder porteria superior
	posicion[2] = { 4,8 }; //Punto de poder porteria inferior
	posicion[3] = { 0,4 }; //Punto de poder banda izquierda
	posicion[4] = { 8,4 }; //Punto de poder banda derecha
}

void Tablero::Inicializar_Partida() {
	
	//Bando 1
	casillas[0][4] = new Portero(1);
	casillas[0][4]->establecer_Posicion(0, 4);

	casillas[0][3] = new Defensa(1);
	casillas[0][3]->establecer_Posicion(0, 3);
	casillas[0][5] = new Defensa(1);
	casillas[0][5]->establecer_Posicion(0, 5);

	casillas[0][1] = new Centrocampistas(1);
	casillas[0][1]->establecer_Posicion(0, 1);
	casillas[0][2] = new Centrocampistas(1);
	casillas[0][2]->establecer_Posicion(0, 2);
	casillas[0][6] = new Centrocampistas(1);
	casillas[0][6]->establecer_Posicion(0, 6);
	casillas[0][7] = new Centrocampistas(1);
	casillas[0][7]->establecer_Posicion(0, 7);

	casillas[0][0] = new Laterales(1);
	casillas[0][0]->establecer_Posicion(0, 0);
	casillas[0][8] = new Laterales(1);
	casillas[0][8]->establecer_Posicion(0, 8);

	casillas[1][0] = new Delantero(1);
	casillas[1][0]->establecer_Posicion(1, 0);
	casillas[1][8] = new Delantero(1);
	casillas[1][8]->establecer_Posicion(1, 8);

	//Bando 2
	casillas[8][4] = new Portero(2);
	casillas[8][4]->establecer_Posicion(8, 4);

	casillas[8][3] = new Defensa(2);
	casillas[8][3]->establecer_Posicion(8, 3);
	casillas[8][5] = new Defensa(2);
	casillas[8][5]->establecer_Posicion(8, 5);

	casillas[8][1] = new Centrocampistas(2);
	casillas[8][1]->establecer_Posicion(8, 1);
	casillas[8][2] = new Centrocampistas(2);
	casillas[8][2]->establecer_Posicion(8, 2);
	casillas[8][6] = new Centrocampistas(2);
	casillas[8][6]->establecer_Posicion(8, 6);
	casillas[8][7] = new Centrocampistas(2);
	casillas[8][7]->establecer_Posicion(8, 7);

	casillas[8][0] = new Laterales(2);
	casillas[8][0]->establecer_Posicion(8, 0);
	casillas[8][8] = new Laterales(2);
	casillas[8][8]->establecer_Posicion(8, 8);

	casillas[7][0] = new Delantero(2);
	casillas[7][0]->establecer_Posicion(7, 0);
	casillas[7][8] = new Delantero(2);
	casillas[7][8]->establecer_Posicion(7, 8);


}

bool Tablero::seleccionar_Pieza(int x, int y) {
	if (x < 0 || x >= 9 || y < 0 || y >= 9)
		return false;

	Pieza* objetivo = casillas[x][y];

	if (objetivo != nullptr) {
		if (objetivo->getBando() == Turno_Actual) {
			pieza_Seleccionada = objetivo;
		std::cout << "Has seleccionado: " << objetivo->getNombre() << " en (" << x << "," << y << ")" << std::endl;
		return true;
		}
		else {
			std::cout << "Esa pieza es del enemigo" << std::endl;
		}
	}
	else {
		std::cout << "Casilla vacía" << std::endl;
	}
	return false;
}

void Tablero::deseleccionar_Pieza() {
	pieza_Seleccionada = nullptr;
}

bool Tablero::mover_Pieza(int dest_x, int dest_y) {

	if (pieza_Seleccionada == nullptr) {
		std::cout << "Error: No hay ninguna pieza seleccionada" << std::endl;
		return false;
	}

	int orig_x = pieza_Seleccionada->getX();
	int orig_y = pieza_Seleccionada->getY();

	if (Verificar_Movimiento(orig_x, orig_y, dest_x, dest_y)) {

		casillas[dest_x][dest_y] = pieza_Seleccionada;

		casillas[orig_x][orig_y] = nullptr;

		pieza_Seleccionada->establecer_Posicion(dest_x, dest_y);

		Avanzar_Turno();

		deseleccionar_Pieza();

		return true;
	}
}

bool Tablero::Verificar_Movimiento(int x1, int y1, int x2, int y2) {

	if (x2 < 0 || x2>8 || y2 < 0 || y2>8) {
		std::cout << "[ERROR MOVIMIENTO]: Te sales del campo "<< std::endl;
		return false;
	} //Se comprueba que el movimiento este en los limites del campo


	if (x1 == x2 && y1 == y2) {
		std::cout << "[ERROR MOVIMIENTO]: Ya estas en esa casilla." << std::endl;
		return false;
	} //Se impide que el destino sea el mismo que el origen
	

	int distancia = abs(x2 - x1) + abs(y2 - y1); // 

	if (distancia > pieza_Seleccionada->getradio()) //Se impide el movimiento si la distancia es mayor que el radio permitido
	{ 
		std::cout << "La pieza no puede llegar ahi." << std::endl;
		return false;
	}
	return true;
}


void Tablero::Avanzar_Turno() {
	if (Turno_Actual == 1) {
		Turno_Actual = 2;
	}
	else {
		Turno_Actual = 1;
	}
	std::cout << " TURNO DEL EQUIPO " << Turno_Actual << std::endl;
}

int Tablero::Comprobar_Ganador() {
	return 0;
}


Tablero::~Tablero() {
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (casillas[i][j] != nullptr) {
				delete casillas[i][j]; // Liberamos el metal
				casillas[i][j] = nullptr;
			}
		}
	}
}

//Borrar no enviar
void Tablero::mostrarTableroprueba() {
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (casillas[i][j] == nullptr) {
				std::cout << " . "; // Espacio vacío
			}
			else {
				// Aquí imprimimos la inicial de la pieza (P, D, C...)
				// Tendrás que crear un método getInicial() en Pieza
				std::cout << " " << casillas[i][j]->getNombre()[0]<<" ";
			}
		}
		std::cout << std::endl;
	}
}