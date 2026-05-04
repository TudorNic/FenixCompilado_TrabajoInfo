#pragma once
#include "Pieza.h"
#include<iostream>

class Delantero : public Pieza {
public:
	Delantero(int bando) :Pieza("Delantero", 120, 10, 0.5, 1, Forma_Movimiento::Tierra, bando) {}
};