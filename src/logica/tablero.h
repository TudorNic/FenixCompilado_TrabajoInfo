#pragma once
#include<vector>


class Jugador;

class Tablero {
	Jugador* casillas[9][9]; //Tablero
	int matrizEfectos[9][9];
	Jugador* pieza_Seleccionada;
	int Turno_Actual;
	int Fase_Ciclo;

	struct Puntos_Poder {
		int x, y;
	};
	Puntos_Poder posicion[5];

	// --- NUEVAS VARIABLES PARA LA INTERCEPCIÓN DEL COMBATE ---
	bool combatePendiente;
	int c_origX, c_origY, c_destX, c_destY;

public:
	Tablero();
	~Tablero();

	void Inicializar_Campo();
	void Inicializar_Partida();
	void reiniciar_Tablero();
	void oscilarTerreno(int Turno);
	int getEfecto_Casilla(int x, int y);
	bool seleccionar_Pieza(int x, int y);
	void deseleccionar_Pieza();
	bool mover_Pieza(int dest_x, int dest_y);
	void Avanzar_Turno();
	void Ejecutar_mejoras(int xOrigen, int yOrigen, int xDestino, int yDestino);
	bool Verificar_Movimiento(int x1, int y1, int x2, int y2);
	int Comprobar_Ganador();

	Puntos_Poder getPuntoPoder(int i) { return posicion[i]; }
	Jugador* getCasilla(int x, int y) { return casillas[x][y]; }
	int getTurnoActual() const { return Turno_Actual; }

	// --- GETTERS PARA QUE EL MAIN SEPA QUE HAY QUE IR A LA ARENA ---
	bool getCombatePendiente() const { return combatePendiente; }
	void resetearCombatePendiente() { combatePendiente = false; }
	int getCOrigX() const { return c_origX; }
	int getCOrigY() const { return c_origY; }
	int getCDestX() const { return c_destX; }
	int getCDestY() const { return c_destY; }

	void aplicar_Resultado_Combate(int orig_x, int orig_y, int dest_x, int dest_y, int bandoGanador);
};