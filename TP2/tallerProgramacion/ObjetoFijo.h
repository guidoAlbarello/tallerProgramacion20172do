#pragma once
#ifndef OBJETOFIJO_H
#define OBJETOFIJO_H

#include "ObjetoDeJuego.h"

enum Posicion { PDerecha = 0, PIzquierda = 1 };

class ObjetoFijo : public ObjetoDeJuego {
public:
	Posicion getPosicion();
	int getUbicacionKM();
	int getTramo() { return this->tramo; }
	void setTramo(int tramo) {
		this->tramo = tramo;
	}
protected:
	Posicion posicion;
	int ubicacionMetros;
	int tramo;
};

#endif
