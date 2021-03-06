#pragma once
#ifndef OBJETO_DE_JUEGO_H
#define OBJETO_DE_JUEGO_H
#include "Vector.h"
#include "Ltexture.h"
#include <string>
#include <vector>

class ObjetoDeJuego {
public:
	virtual void update(Unidad delta);
	virtual void leerEntrada(std::vector<SDL_Keycode> entradas);
	virtual void leerEntrada();
	void inicializar(string path);
	ObjetoDeJuego();
	ObjetoDeJuego(SDL_Renderer* renderer);
	~ObjetoDeJuego();
protected:
	Ltexture* texture;
	Vector posicion;
};

#endif