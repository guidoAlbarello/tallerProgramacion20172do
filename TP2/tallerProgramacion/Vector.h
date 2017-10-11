#pragma once
#ifndef VECTOR_H
#define VECTOR_H
#include "Constantes.h"

class Vector {
public:
	Vector();
	Vector(Unidad x, Unidad y);
	Unidad getX() { return x; }
	Unidad getY() { return y; }
	void setX(Unidad x) { this->x = x; }
	void setY(Unidad y) { this->y = y; }
private:
	Unidad x;
	Unidad y;
};

#endif