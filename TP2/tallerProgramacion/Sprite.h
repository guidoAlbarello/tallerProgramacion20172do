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
	void setPosicionInt(int x, int y) { this->x = x; this->y = y; }
	void setAnchoYAlto(int ancho, int alto) { this->ancho = ancho; this->alto = alto; }
	void setId(std::string id) { this->id = id; };
private:
	int ancho, alto, filaActual, frameActual, x,y;
	std::string id;
};

#endif