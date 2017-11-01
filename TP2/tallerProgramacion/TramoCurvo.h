#pragma once
#ifndef TRAMOCURVO_H
#define TRAMOCURVO_H

#include "Tramo.h"

enum SentidoCurva { SCIzquierda = 0, SCDerecha = 1, NINGUNO = 2 };

class TramoCurvo : public Tramo {
public:
	SentidoCurva getSentido();
	int getLongitud() {
		return this->longitud;
	}
	TramoCurvo(int longitud, SentidoCurva sentido);
	TramoCurvo();
private:
	SentidoCurva sentido;
};

#endif