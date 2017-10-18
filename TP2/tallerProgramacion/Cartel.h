#pragma once
#ifndef OBJETOCARTEL_H
#define OBJETOCARTEL_H

#include "ObjetoFijo.h"

enum MaximaVelocidad { V60 = 0, V80 = 1, V120 = 2 };

class Cartel : public ObjetoFijo {
public:
	MaximaVelocidad getMaximaVelocidad();
	Cartel(int ubicacionKM, Posicion posicion, MaximaVelocidad maximaVelocidad);
private:
	MaximaVelocidad maximaVelocidad;
};

#endif