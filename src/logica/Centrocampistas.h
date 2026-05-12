#pragma once
#include "Pieza.h"
#include<iostream>

class Centrocampistas : public Pieza {
public:
	Centrocampistas(int bando) :Pieza("Centrocampistas", 120, 10, 0.5, 1, Forma_Movimiento::Tierra, bando) {}
};