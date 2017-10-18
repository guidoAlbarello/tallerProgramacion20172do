#pragma once
#ifndef OBJETOFIJO_H
#define OBJETOFIJO_H

#include "ObjetoDeJuego.h"

enum Posicion { PDerecha = 0, PIzquierda = 1 };

class ObjetoFijo : public ObjetoDeJuego {
public:
	Posicion getPosicion();
	int getUbicacionKM();
protected:
	Posicion posicion;
	int ubicacionKM;
};

#endif#pragma once
