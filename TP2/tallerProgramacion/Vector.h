#pragma once
#include "Constantes.h"

class Vector {
public:
	Unidad getX() { return x; }
	Unidad getY() { return y; }
	void setX(Unidad x) { this->x = x; }
	void setY(Unidad y) { this->y = y; }
private:
	Unidad x;
	Unidad y;
};