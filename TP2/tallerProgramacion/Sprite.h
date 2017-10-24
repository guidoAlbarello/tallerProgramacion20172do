#pragma once

#ifndef SPRITE_H
#define SPRITE_H
#include "ObjetoDeJuego.h"
#include "Renderer.h"
#include "ManejadorDeTexturas.h"

class Renderer;

class Sprite : public ObjetoDeJuego {
public:
	void renderStatic(Renderer* renderer);
	void load(std::string fileName,SDL_Renderer* pRenderer);
	void render(Renderer* renderer);
	void setFilaActual(int fila) { filaActual = fila; };
	void setFrameActual(int frame) { frameActual = frame; };
private:
	int ancho, alto, filaActual, frameActual;
	std::string id;
};

#endif