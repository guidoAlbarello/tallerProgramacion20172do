#pragma once
#ifndef ESCENARIO_H
#define ESCENARIO_H

#define VELOCIDAD_CIELO 0.1
#define VELOCIDAD_COLINAS 0.05

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
	void setPosicionCielo(int x, int y);
	void setPosicionColinas(int x, int y);
	bool limiteImagenCielo() { return posicionCielo.getX() < 800; }
	bool limiteImagenColinas() { return posicionColinas.getX() < 800; }
private:
	Vector posicionCielo;
	Vector posicionColinas;
	Sprite* cielo;
	Sprite* colinas;
	Renderer* renderer;
};
#endif