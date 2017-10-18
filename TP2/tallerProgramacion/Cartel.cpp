#include "Cartel.h"

MaximaVelocidad Cartel::getMaximaVelocidad() {
	return this->maximaVelocidad;
}

Cartel::Cartel(int ubicacionKM, Posicion posicion, MaximaVelocidad maximaVelocidad) {
	this->ubicacionKM = ubicacionKM;
	this->posicion = posicion;
	this->maximaVelocidad = maximaVelocidad;
}
