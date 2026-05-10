#pragma once
#include<vector>

class Jugador;

class Tablero {
	Jugador* casillas[9][9]; //Tablero
	Jugador* pieza_Seleccionada;

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

	int Ejecutar_combate(Jugador* atacante, Jugador* defensor);

	bool Verificar_Movimiento(int x1, int y1, int x2, int y2);

	int Comprobar_Ganador();

	Jugador* getCasilla(int x, int y) { return casillas[x][y]; }
	int getTurnoActual() const { return Turno_Actual; }
};