#pragma once
#ifndef ESCENARIO_H
#define ESCENARIO_H

#define VELOCIDAD_CIELO 2.0
#define VELOCIDAD_COLINAS 1.0

#include "Sprite.h"
#include "Vector.h"
#include "Constantes.h"

class Escenario {
public:
	Escenario();
	Escenario(Renderer* renderer);
	void iniciar();
	~Escenario();
	void update(Unidad deltaTiempo);
	void render();
	Vector* getPosicionCielo() { return &posicionCielo; }
	Vector* getPosicionColinas() { return &posicionColinas; }
	void setPosicionCielo(int x, int y) { posicionCielo.setX(x); posicionCielo.setY(y); }
	void setPosicionColinas(int x, int y) { posicionColinas.setX(x); posicionColinas.setY(y); }
private:
	Vector posicionCielo;
	Vector posicionColinas;
	Sprite* cielo;
	Sprite* colinas;
	Renderer* renderer;
};
#endif