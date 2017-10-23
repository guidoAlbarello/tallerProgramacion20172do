#pragma once
#ifndef OBJETO_DE_JUEGO_H
#define OBJETO_DE_JUEGO_H
#include "Vector.h"
#include "Ltexture.h"
#include <string>

class ObjetoDeJuego {
public:
	virtual void update(Unidad delta);
	virtual void leerEntrada();
	void inicializar(string path);
	ObjetoDeJuego();
	ObjetoDeJuego(SDL_Renderer* renderer);
	int getZIndex() { return this->zIndex; }
	~ObjetoDeJuego();
protected:
	Ltexture* texture;
	Vector posicion;
	Vector velocidad;
	int zIndex;
};

#endif