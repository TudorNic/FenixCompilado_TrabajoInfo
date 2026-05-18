#pragma once
#include<string>
//Definición de la clase general pieza para dar lugar a cada una con sus atributos
enum class Forma_Movimiento{Tierra,Vuelo,Teletrasnporte};
class Pieza
{
	//Atributo de la pieza
	std::string nombre;
	int vida_actual;
	int vida_maxima;
	int fuerza;
	double velocidad_ataque;
	int radio_movimiento;

	//Capacidad de movimiento o categoia general: Tierra Vuelo Teletransporte
	Forma_Movimiento mov;

	int bando; //Bando de la pieza

	int pos_x, pos_y; //Posición personal de la pieza

public:
	Pieza(std::string nom, int vida, int f, double vel, int radio, Forma_Movimiento t,int equipo);

	std::string getNombre() { return nombre;}//Borrar no enviar

	virtual ~Pieza(); //Destructor que limpia todo lo relacionado a la clase especifica,evitando fugas de memorias

	virtual void ejecutar_Habilidad_especial();

	int getVida()const { return vida_actual; }

	int getBando() { return bando; }

	int getX() const { return pos_x; }

	int getY() const { return pos_y; }

	int getradio() const { return radio_movimiento; }

	Forma_Movimiento getForma_Mov() const { return mov; }

	void recibir_Danio(int cantidad);

	void establecer_Posicion(int x, int y);
};

