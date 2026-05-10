#include "tablero.h"
#include "Entrenador.h"
#include "Defensa.h"
#include "Delantero.h"
#include "Lateral.h"
#include "Centrocampista.h"
#include "Jugador.h"
#include "Aficion.h"
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
	casillas[0][4] = new Entrenador(1);
	casillas[0][4]->establecer_Posicion(0, 4);

	casillas[0][3] = new Defensa(1);
	casillas[0][3]->establecer_Posicion(0, 3);
	casillas[0][5] = new Defensa(1);
	casillas[0][5]->establecer_Posicion(0, 5);

	casillas[0][1] = new Centrocampista(1);
	casillas[0][1]->establecer_Posicion(0, 1);
	casillas[0][2] = new Centrocampista(1);
	casillas[0][2]->establecer_Posicion(0, 2);
	casillas[0][6] = new Centrocampista(1);
	casillas[0][6]->establecer_Posicion(0, 6);
	casillas[0][7] = new Centrocampista(1);
	casillas[0][7]->establecer_Posicion(0, 7);

	casillas[0][0] = new Lateral(1);
	casillas[0][0]->establecer_Posicion(0, 0);
	casillas[0][8] = new Lateral(1);
	casillas[0][8]->establecer_Posicion(0, 8);

	casillas[1][0] = new Delantero(1);
	casillas[1][0]->establecer_Posicion(1, 0);
	casillas[1][8] = new Delantero(1);
	casillas[1][8]->establecer_Posicion(1, 8);

	//Bando 2
	casillas[8][4] = new Entrenador(2);
	casillas[8][4]->establecer_Posicion(8, 4);

	casillas[8][3] = new Defensa(2);
	casillas[8][3]->establecer_Posicion(8, 3);
	casillas[8][5] = new Defensa(2);
	casillas[8][5]->establecer_Posicion(8, 5);

	casillas[8][1] = new Centrocampista(2);
	casillas[8][1]->establecer_Posicion(8, 1);
	casillas[8][2] = new Centrocampista(2);
	casillas[8][2]->establecer_Posicion(8, 2);
	casillas[8][6] = new Centrocampista(2);
	casillas[8][6]->establecer_Posicion(8, 6);
	casillas[8][7] = new Centrocampista(2);
	casillas[8][7]->establecer_Posicion(8, 7);

	casillas[8][0] = new Lateral(2);
	casillas[8][0]->establecer_Posicion(8, 0);
	casillas[8][8] = new Lateral(2);
	casillas[8][8]->establecer_Posicion(8, 8);

	casillas[7][0] = new Delantero(2);
	casillas[7][0]->establecer_Posicion(7, 0);
	casillas[7][8] = new Delantero(2);
	casillas[7][8]->establecer_Posicion(7, 8);

	//Bando 1 y 2 aficion
	for(int i=1;i<8;i++)
	{
		casillas[i][1] = new Aficion(1);
		casillas[i][1]->establecer_Posicion(i, 1);

		casillas[i][7] = new Aficion(2);
		casillas[i][7]->establecer_Posicion(i, 7);
	}


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

bool Tablero::mover_Pieza(int dest_x, int dest_y) {

	if (pieza_Seleccionada == nullptr) {
		std::cout << "Error: No hay ninguna pieza seleccionada" << std::endl;
		return false;
	}

	int orig_x = pieza_Seleccionada->getX();
	int orig_y = pieza_Seleccionada->getY();

	if (Verificar_Movimiento(orig_x, orig_y, dest_x, dest_y)) 
	{
		
		if (casillas[dest_x][dest_y] != nullptr) {
			// LLamada al combate que debe devolver un ganador 
		}

		if (casillas[dest_x][dest_y] == nullptr /*&& La llamada al combate devuleve victoria para el atacante,el defesnor se elimina dentro de la funcion */)
		{

			casillas[dest_x][dest_y] = pieza_Seleccionada;

			casillas[orig_x][orig_y] = nullptr;

			pieza_Seleccionada->establecer_Posicion(dest_x, dest_y);

			Avanzar_Turno();

			deseleccionar_Pieza();

			return true;
		}
		else {
			if (false/*La llamada al combate devuelve derrota para el atacante*/) 
			{
				delete casillas[orig_x][orig_y];
				casillas[orig_x][orig_y] = nullptr;
				Avanzar_Turno();
				deseleccionar_Pieza();
			}

		}
	}
	return false;
}

bool Tablero::Verificar_Movimiento(int x1, int y1, int x2, int y2) {

	if (x2 < 0 || x2>8 || y2 < 0 || y2>8) {
		return false;
	} //Se comprueba que el movimiento este en los limites del campo


	if (x1 == x2 && y1 == y2) {
		return false;
	} //Se impide que el destino sea el mismo que el origen

	Jugador* p = casillas[x1][y1];

	if (p == nullptr) return false;

	int distancia = abs(x2 - x1) + abs(y2 - y1); 

	if (distancia > p->getRadio()) //Se impide el movimiento si la distancia es mayor que el radio permitido
	{ 
		
		return false;
	}

	if(casillas[x2][y2] != nullptr)
	{
		if (casillas[x2][y2]->getBando() == p->getBando()) //Se impide que un aliado pise a otro
		{
			return false;
		}
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
	int piezas_1=0, piezas_2=0;

	for (int i = 0; i < 9;i++) 
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
	if (piezas_1 == 0) return 2;
	if (piezas_2 == 0) return 1;


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

	if (puntos_Poder1 == 5) return 1;
	if (puntos_Poder2 == 5) return 2;

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
				std::cout << " " << casillas[i][j]->getNombreClase()[0]<<" ";
			}
		}
		std::cout << std::endl;
	}
}