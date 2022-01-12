#include "lab_m1/Tema2/proiectil.h"
#include <fstream>
#include <string>
#include <iostream>

Proiectil::Proiectil()
{

	lifeTime = 1.0f;
	shoot = false;
	speed = 0;
	velocity = 10;

}

Proiectil::~Proiectil()
{

}