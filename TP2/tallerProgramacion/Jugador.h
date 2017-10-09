#pragma once
#include "ObjetoDeJuego.h"
class Jugador : public ObjetoDeJuego {  //tal vez diferenciar entre movil y estatico
public:
	void recibirEntrada(int pos, bool estadoEntrada);
	void setId(int id) { this->id = id; }
	int getId() { return this->id; }
protected:
	bool entrada[3];
	int id;
};