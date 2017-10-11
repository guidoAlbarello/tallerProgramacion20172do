#pragma once
#ifndef OBJETO_DE_JUEGO_H
#define OBJETO_DE_JUEGO_H
#include "Vector.h"

class ObjetoDeJuego {
public:
	virtual void update(Unidad delta);
	virtual void leerEntrada();
protected:
	Vector posicion;
};

#endif