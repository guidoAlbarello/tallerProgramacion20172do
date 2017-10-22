#pragma once
#ifndef JUGADOR_H
#define JUGADOR_H
#include "ObjetoDeJuego.h"
class Jugador : public ObjetoDeJuego {  //tal vez diferenciar entre movil y estatico
public:
	Jugador(SDL_Renderer* renderer);
	void update(Unidad delta);
	void recibirEntrada(int pos, bool estadoEntrada);
	void setId(int id) { this->id = id; }
	int getId() { return this->id; }
protected:
	bool entrada[3];
	int id;
};
#endif