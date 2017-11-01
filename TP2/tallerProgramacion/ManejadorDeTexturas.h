#pragma once
#ifndef MANEJADOR_DE_TEXTURAS_H
#define MANEJADOR_DE_TEXTURAS_H

#define MINIMO_ANCHO 30


#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <map>
#include "Camara.h"
#include <stdlib.h>  
#include <math.h>
#include "SDL2_gfxPrimitives.h"
#include "Constantes.h"

class ManejadorDeTexturas {
public:
	static ManejadorDeTexturas* getInstance();
	void drawAnimatedSprite(std::string id, int x, int y, int ancho, int alto, int filaActual, int frameActual, int anchoPantalla, int altoPantalla,
			SDL_Renderer *pRenderer, SDL_RendererFlip flip);
	void drawStaticSprite(std::string id, int x, int y, int ancho, int alto, int anchoPantalla, SDL_Renderer* pRenderer, SDL_RendererFlip flip, float xx);
	void dibujarSprite(std::string id, int x, int y, int ancho, int alto, int anchoPantalla, SDL_Renderer* pRenderer, SDL_RendererFlip flip);
	bool load(std::string fileName, std::string	id, SDL_Renderer* pRenderer);
	void setCamara(Camara* unaCamara) { this->camara = unaCamara; }
	void dibujarTramo(Segmento* unSegmento,int ancho, int anchoPantalla, int altoPantalla, SDL_Renderer* renderer, int n, float x);
	Camara* getCamara() { return camara; }
private:
	Camara* camara;
	float normZIndex(float zIndex);
	static ManejadorDeTexturas* instance;
	ManejadorDeTexturas();
	~ManejadorDeTexturas();
	void proyectar(Coordenada& p, int& ancho, int anchoPantalla, int altoPantalla,float x);
	std::map<std::string, SDL_Texture*> texturas;
};

#endif