#pragma once
#include "Vector.h"

class ObjetoDeJuego {
public:
	void update(Unidad delta);
	void leerEntrada();
protected:
	Vector posicion;
};