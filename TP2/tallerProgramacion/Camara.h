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
private:
	ObjetoDeJuego* target;
	Vector* posicion;
};

#endif