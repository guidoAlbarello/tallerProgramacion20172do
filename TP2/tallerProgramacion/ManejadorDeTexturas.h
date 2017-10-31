#pragma once
#ifndef MANEJADOR_DE_TEXTURAS_H
#define MANEJADOR_DE_TEXTURAS_H

#define Z_FAR 800
#define Z_NEAR 0


#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <map>
#include "Camara.h"
#include <stdlib.h>  
#include <math.h>
class ManejadorDeTexturas {
public:
	static ManejadorDeTexturas* getInstance();
	void drawAnimatedSprite(std::string id, int x, int y, int ancho, int alto, int filaActual, int frameActual, int anchoPantalla, int altoPantalla,
			SDL_Renderer *pRenderer, SDL_RendererFlip flip);
	void drawStaticSprite(std::string id, int x, int y, int ancho, int alto, int anchoPantalla, SDL_Renderer* pRenderer, SDL_RendererFlip flip);
	void dibujarSprite(std::string id, int x, int y, int ancho, int alto, int anchoPantalla, SDL_Renderer* pRenderer, SDL_RendererFlip flip);
	bool load(std::string fileName, std::string	id, SDL_Renderer* pRenderer);
	void setCamara(Camara* unaCamara) { this->camara = unaCamara; }
	void dibujarTramo(int x, int y, int ancho, int alto, int anchoPantalla, int altoPantalla, SDL_Renderer* renderer, bool grisOscuro);
private:
	Camara* camara;
	float normZIndex(float zIndex);
	static ManejadorDeTexturas* instance;
	ManejadorDeTexturas();
	~ManejadorDeTexturas();
	std::map<std::string, SDL_Texture*> texturas;
};

#endif