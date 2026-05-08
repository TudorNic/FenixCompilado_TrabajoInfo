#include "Pieza.h"
Pieza::Pieza(std::string nom, int vida, int f, double vel, int radio,Forma_Movimiento t,int equipo)
	:nombre(nom),vida_actual(vida),vida_maxima(vida),fuerza(f),velocidad_ataque(vel),
	radio_movimiento(radio),mov(t),bando(equipo),pos_x(0),pos_y(0)
{
} 

Pieza::~Pieza() {

}

void Pieza::recibir_Danio(int cantidad) {

	vida_actual -= cantidad;

	if (vida_actual < 0)
		vida_actual = 0;
}

void Pieza::establecer_Posicion(int x, int y) {
	pos_x = x;
	pos_y = y;
}

void Pieza::ejecutar_Habilidad_especial()
{
}