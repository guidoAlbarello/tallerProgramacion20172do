#include "Camara.h"

Camara::Camara() {
	this->target = NULL;
	this->posicion = new Vector(0,0);
}

void Camara::update() {

}

Vector * Camara::getPosicionTarget() {
	return target->getPosicion();
}
