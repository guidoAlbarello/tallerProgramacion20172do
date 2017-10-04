#pragma once
#include "ObjetoDeJuego.h"
class Jugador : public ObjetoDeJuego {  //tal vez diferenciar entre movil y estatico
public:
	void recibirEntrada(int pos, bool estadoEntrada);
protected:
	bool entrada[3];
};