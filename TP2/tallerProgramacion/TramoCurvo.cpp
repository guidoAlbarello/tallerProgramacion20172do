#include "TramoCurvo.h"

SentidoCurva TramoCurvo::getSentido() {
	return this->sentido;
}

TramoCurvo::TramoCurvo() {

}

TramoCurvo::TramoCurvo(int longitud, SentidoCurva sentido) {
	this->longitud = longitud;
	this->sentido = sentido;
	this->tipo = TipoTramo::Curva;
}

