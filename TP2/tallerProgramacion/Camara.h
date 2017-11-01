#pragma once
#ifndef CAMARA_H
#define CAMARA_H

#include "ObjetoDeJuego.h"
#include "Vector.h"

class Camara {
public:
	Camara();
	void setTarget(ObjetoDeJuego* target) { this->target = target; }
	Vector* getPosicion() { return this->posicion; }
	void update();
	void setPosicion(int x, int y) { posicion->setX(x); posicion->setY(y); }
	Vector* getPosicionTarget();
	void addX(Unidad dx) { posicion->setX(posicion->getX() + dx); }
private:
	ObjetoDeJuego* target;
	Vector* posicion;
};

#endif