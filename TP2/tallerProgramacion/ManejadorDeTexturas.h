#pragma once
#ifndef MANEJADOR_DE_TEXTURAS_H
#define MANEJADOR_DE_TEXTURAS_H

#define FACTOR_PERSPECTIVA 0.2

#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <map>

class ManejadorDeTexturas {
public:
	static ManejadorDeTexturas* getInstance();
	void drawAnimatedSprite(std::string id, int x, int y, int ancho, int alto, int filaActual, int frameActual, int posicionCamara, int anchoPantalla, int zIndex,
			SDL_Renderer *pRenderer, SDL_RendererFlip flip);
	void drawStaticSprite(std::string id, int x, int y, int ancho, int alto, int posicionCamara, int anchoPantalla, int zIndex,
			SDL_Renderer* pRenderer, SDL_RendererFlip flip);
	bool ManejadorDeTexturas::load(std::string fileName, std::string	id, SDL_Renderer* pRenderer);
private:
	static ManejadorDeTexturas* instance;
	ManejadorDeTexturas();
	~ManejadorDeTexturas();
	std::map<std::string, SDL_Texture*> texturas;
};

#endif