#include "tablero.h"
#include "Entrenador.h"
#include "Defensa.h"
#include "Delantero.h"
#include "Lateral.h"
#include "Centrocampista.h"
#include "Jugador.h"
#include "Aficion.h"
#include "math.h"
#include <algorithm>
#include <iostream>

// Constructor actualizado con la inicialización de las banderas de la Arena
Tablero::Tablero() : Turno_Actual(1), Fase_Ciclo(0), combatePendiente(false), c_origX(-1), c_origY(-1), c_destX(-1), c_destY(-1) {
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			casillas[i][j] = nullptr;
			matrizEfectos[i][j] = 0;
		}
	}
	Inicializar_Campo();
	Inicializar_Partida();
	pieza_Seleccionada = nullptr;
}

void Tablero::Inicializar_Campo() {
	posicion[0] = { 4,4 }; // Punto de poder centro del campo
	posicion[1] = { 4,0 }; // Punto de poder porteria superior
	posicion[2] = { 4,8 }; // Punto de poder porteria inferior
	posicion[3] = { 0,4 }; // Punto de poder banda izquierda
	posicion[4] = { 8,4 }; // Punto de poder banda derecha
	oscilarTerreno(Turno_Actual);

	for (int i = 0; i < 5; i++) {
		matrizEfectos[posicion[i].x][posicion[i].y] = 3;
	}
}

void Tablero::Inicializar_Partida() {
	// Bando 1
	casillas[4][8] = new Entrenador(1);
	casillas[4][8]->establecer_Posicion(4, 8);

	casillas[3][8] = new Defensa(1);
	casillas[3][8]->establecer_Posicion(3, 8);
	casillas[5][8] = new Defensa(1);
	casillas[5][8]->establecer_Posicion(5, 8);

	casillas[1][8] = new Centrocampista(1);
	casillas[1][8]->establecer_Posicion(1, 8);
	casillas[2][8] = new Centrocampista(1);
	casillas[2][8]->establecer_Posicion(2, 8);
	casillas[6][8] = new Centrocampista(1);
	casillas[6][8]->establecer_Posicion(6, 8);
	casillas[7][8] = new Centrocampista(1);
	casillas[7][8]->establecer_Posicion(7, 8);

	casillas[0][8] = new Lateral(1);
	casillas[0][8]->establecer_Posicion(0, 8);
	casillas[8][8] = new Lateral(1);
	casillas[8][8]->establecer_Posicion(8, 8);

	casillas[0][7] = new Delantero(1);
	casillas[0][7]->establecer_Posicion(0, 7);
	casillas[8][7] = new Delantero(1);
	casillas[8][7]->establecer_Posicion(8, 7);

	// Bando 2
	casillas[4][0] = new Entrenador(2);
	casillas[4][0]->establecer_Posicion(4, 0);

	casillas[3][0] = new Defensa(2);
	casillas[3][0]->establecer_Posicion(3, 0);
	casillas[5][0] = new Defensa(2);
	casillas[5][0]->establecer_Posicion(5, 0);

	casillas[1][0] = new Centrocampista(2);
	casillas[1][0]->establecer_Posicion(1, 0);
	casillas[2][0] = new Centrocampista(2);
	casillas[2][0]->establecer_Posicion(2, 0);
	casillas[6][0] = new Centrocampista(2);
	casillas[6][0]->establecer_Posicion(6, 0);
	casillas[7][0] = new Centrocampista(2);
	casillas[7][0]->establecer_Posicion(7, 0);

	casillas[0][0] = new Lateral(2);
	casillas[0][0]->establecer_Posicion(0, 0);
	casillas[8][0] = new Lateral(2);
	casillas[8][0]->establecer_Posicion(8, 0);

	casillas[0][1] = new Delantero(2);
	casillas[0][1]->establecer_Posicion(0, 1);
	casillas[8][1] = new Delantero(2);
	casillas[8][1]->establecer_Posicion(8, 1);

	// Bando 1 y 2 aficion
	for (int i = 1; i < 8; i++)
	{
		casillas[i][7] = new Aficion(1);
		casillas[i][7]->establecer_Posicion(i, 7);

		casillas[i][1] = new Aficion(2);
		casillas[i][1]->establecer_Posicion(i, 1);
	}
}

struct Coordenada { int x, y; };
Coordenada casillasCambiantes[27] = {
	{0,3}, {1,3}, {2,3}, {3,3}, {4,3}, {5,3}, {6,3}, {7,3}, {8,3},
	{0,4}, {1,4}, {2,4}, {3,4}, {4,4}, {5,4}, {6,4}, {7,4}, {8,4},
	{0,5}, {1,5}, {2,5}, {3,5}, {4,5}, {5,5}, {6,5}, {7,5}, {8,5}
};

void Tablero::oscilarTerreno(int turno_actual) {
	int fase = turno_actual % 2;

	if (fase == 0)
	{
		for (int c = 0; c < 9; c++)
		{
			matrizEfectos[c][2] = 1;
			matrizEfectos[c][4] = 2;
			matrizEfectos[c][6] = 1;
		}
	}
	else
	{
		for (int c = 0; c < 9; c++)
		{
			matrizEfectos[c][2] = 2;
			matrizEfectos[c][4] = 1;
			matrizEfectos[c][6] = 2;
		}
	}
}

int Tablero::getEfecto_Casilla(int x, int y) {
	if (x >= 0 && x < 9 && y >= 0 && y < 9) {
		return matrizEfectos[x][y];
	}
	return 0;
}

bool Tablero::seleccionar_Pieza(int x, int y) {
	if (x < 0 || x >= 9 || y < 0 || y >= 9)
		return false;

	Jugador* objetivo = casillas[x][y];

	if (objetivo != nullptr) {
		if (objetivo->getBando() == Turno_Actual) {
			pieza_Seleccionada = objetivo;
			std::cout << "Has seleccionado: " << objetivo->getNombreClase() << " en (" << x << "," << y << ")" << std::endl;
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

// Lógica de movimiento actualizada para interceptar peleas e ir a la Arena 2D
bool Tablero::mover_Pieza(int dest_x, int dest_y) {
	if (pieza_Seleccionada == nullptr) {
		return false;
	}

	int orig_x = pieza_Seleccionada->getX();
	int orig_y = pieza_Seleccionada->getY();

	if (Verificar_Movimiento(orig_x, orig_y, dest_x, dest_y))
	{
		// ¡INTERCEPCIÓN GLOBAL! Si hay una pieza enemiga en el destino, congelamos y activamos bandera
		if (casillas[dest_x][dest_y] != nullptr)
		{
			combatePendiente = true;
			c_origX = orig_x;
			c_origY = orig_y;
			c_destX = dest_x;
			c_destY = dest_y;
			return true;
		}

		// Movimiento pacífico si la casilla está vacía
		if (casillas[dest_x][dest_y] == nullptr)
		{
			casillas[dest_x][dest_y] = pieza_Seleccionada;
			casillas[orig_x][orig_y] = nullptr;
			pieza_Seleccionada->establecer_Posicion(dest_x, dest_y);

			Avanzar_Turno();
			deseleccionar_Pieza();
			Comprobar_Ganador();
			return true;
		}
	}
	return false;
}

bool Tablero::Verificar_Movimiento(int x1, int y1, int x2, int y2) {
	if (x1 < 0 || x1 >= 9 || y1 < 0 || y1 >= 9 || x2 < 0 || x2 >= 9 || y2 < 0 || y2 >= 9) {
		return false;
	}
	Jugador* p = casillas[x1][y1];

	if (p == nullptr) return false;

	int diffX = abs(x2 - x1);
	int diffY = abs(y2 - y1);
	int distancia = std::max(diffX, diffY);

	if (distancia > p->getRadio())
	{
		std::cout << "Error: Distancia no permitida" << std::endl;
		return false;
	}

	if (casillas[x2][y2] != nullptr)
	{
		if (casillas[x2][y2]->getBando() == casillas[x1][y1]->getBando())
		{
			return false;
		}
	}

	return true;
}

void Tablero::Avanzar_Turno() {
	if (Turno_Actual == 1) {
		Turno_Actual = 2;
		oscilarTerreno(2);
	}
	else {
		Turno_Actual = 1;
		oscilarTerreno(1);
	}
	std::cout << " TURNO DEL EQUIPO " << Turno_Actual << std::endl;
}

int Tablero::Comprobar_Ganador() {
	int piezas_1 = 0, piezas_2 = 0;

	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if (casillas[i][j] != nullptr)
			{
				if (casillas[i][j]->getBando() == 1) piezas_1++;
				else piezas_2++;
			}
		}
	}

	int puntos_Poder1 = 0;
	int puntos_Poder2 = 0;

	for (int i = 0; i < 5; i++) {
		int px = posicion[i].x;
		int py = posicion[i].y;

		if (casillas[px][py] != nullptr) {
			if (casillas[px][py]->getBando() == 1) puntos_Poder1++;
			else if (casillas[px][py]->getBando() == 2) puntos_Poder2++;
		}
	}

	if (puntos_Poder1 == 5) {
		std::cout << "GANADOR: BANDO 1 (Puntos de Poder)" << std::endl;
		return 1;
	}
	if (puntos_Poder2 == 5) {
		std::cout << "GANADOR: BANDO 2 (Puntos de Poder)" << std::endl;
		return 2;
	}

	if (piezas_1 > 0 && piezas_2 == 0) {
		std::cout << "GANADOR: BANDO 1 (Exterminio)" << std::endl;
		return 1;
	}
	if (piezas_2 > 0 && piezas_1 == 0) {
		std::cout << "GANADOR: BANDO 2 (Exterminio)" << std::endl;
		return 2;
	}

	return 0;
}

Tablero::~Tablero() {
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (casillas[i][j] != nullptr) {
				delete casillas[i][j];
				casillas[i][j] = nullptr;
			}
		}
	}
}

// Lógica de simulación de Hugo antigua (se mantiene por si se pide)
bool Tablero::Ejecutar_combate(Jugador* atacante, Jugador* defensor) {
	if (defensor == nullptr) return true;

	float Dano_Modificado = defensor->getDanoAtaque();
	float Velocidad_Modificada = defensor->getVelocidad();

	if (Turno_Actual == 1) {
		if (defensor->getY() == 2 || defensor->getY() == 6)
		{
			Velocidad_Modificada = defensor->getVelocidad() * 1.3f;
			Dano_Modificado = defensor->getDanoAtaque() * 0.7f;
		}
		else if (defensor->getY() == 4)
		{
			Velocidad_Modificada = defensor->getVelocidad() * 0.7f;
			Dano_Modificado = defensor->getDanoAtaque() * 1.3f;
		}
	}
	else if (Turno_Actual == 2) {
		if (defensor->getY() == 2 || defensor->getY() == 6)
		{
			Velocidad_Modificada = defensor->getVelocidad() * 0.7f;
			Dano_Modificado = defensor->getDanoAtaque() * 1.3f;
		}
		else if (defensor->getY() == 4)
		{
			Velocidad_Modificada = defensor->getVelocidad() * 1.3f;
			Dano_Modificado = defensor->getDanoAtaque() * 0.7f;
		}
	}

	if (Dano_Modificado > atacante->getDanoAtaque())
		return false;
	else
		return true;
}

// NUEVA FUNCIÓN MAESTRA: Aplica las bajas reales del combate 2D al tablero táctico
void Tablero::aplicar_Resultado_Combate(int orig_x, int orig_y, int dest_x, int dest_y, int bandoGanador) {
	if (casillas[orig_x][orig_y] == nullptr || casillas[dest_x][dest_y] == nullptr) return;

	int bandoAtacante = casillas[orig_x][orig_y]->getBando();



	if (bandoGanador == bandoAtacante) {
		// Gana el que inició el movimiento: el defensor muere y el atacante avanza
		delete casillas[dest_x][dest_y];
		casillas[dest_x][dest_y] = casillas[orig_x][orig_y];
		casillas[orig_x][orig_y] = nullptr;
		casillas[dest_x][dest_y]->establecer_Posicion(dest_x, dest_y);
		std::cout << "[TABLERO] El atacante gana la plaza." << std::endl;
	}
	else {
		// Gana el defensor: el atacante es destruido en su casilla de origen
		delete casillas[orig_x][orig_y];
		casillas[orig_x][orig_y] = nullptr;
		std::cout << "[TABLERO] El defensor repele el ataque. Ficha atacante destruida." << std::endl;
	}

	Avanzar_Turno();
	deseleccionar_Pieza();
	Comprobar_Ganador();
}

void Tablero::reiniciar_Tablero() 
{
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if (casillas[i][j] != nullptr)
			{
				delete casillas[i][j];
				casillas[i][j] = nullptr;
			}
		}

		combatePendiente = false;
		pieza_Seleccionada = nullptr;
		Turno_Actual = 1;
		Inicializar_Campo();
		Inicializar_Partida();
	}
}