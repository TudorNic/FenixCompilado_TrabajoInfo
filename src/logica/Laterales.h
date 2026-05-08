#pragma once
#include "Pieza.h"
#include<iostream>

class Laterales : public Pieza {
public:
	Laterales(int bando) :Pieza("Laterales", 120, 10, 0.5, 1, Forma_Movimiento::Tierra, bando) {}
};