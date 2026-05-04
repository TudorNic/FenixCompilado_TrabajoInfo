#pragma once
#include<vector>

class Pieza;

class Tablero {
	Pieza* casillas[9][9]; //Tablero
	Pieza* pieza_Seleccionada;

	int Turno_Actual;

	int Fase_Ciclo;

	struct Puntos_Poder {
		int x, y;
	};

	Puntos_Poder posicion[5];

public:
	Tablero();
	~Tablero();

	void mostrarTableroprueba();

	void Inicializar_Campo();

	void Inicializar_Partida();

	bool seleccionar_Pieza(int x, int y);

	void deseleccionar_Pieza();

	bool mover_Pieza(int dest_x, int dest_y);

	void Avanzar_Turno();

	bool Verificar_Movimiento(int x1, int y1, int x2, int y2);

	int Comprobar_Ganador();
};

