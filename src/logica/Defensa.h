#pragma once
#include "Pieza.h"
#include<iostream>

class Defensa : public Pieza {
public:
	Defensa(int bando) :Pieza("Defensa", 80, 15, 0.8, 2, Forma_Movimiento::Tierra, bando) {}
};