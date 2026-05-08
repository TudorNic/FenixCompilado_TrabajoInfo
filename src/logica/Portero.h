#pragma once
#include "Pieza.h"
#include<iostream>

class Portero: public Pieza {
public:
	Portero(int bando) :Pieza ("Portero",40,30,1.5 ,1,Forma_Movimiento::Tierra,bando){}
};