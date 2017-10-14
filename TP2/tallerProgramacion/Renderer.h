#pragma once

#ifndef RENDERER_H
#define RENDERER_H
#include <thread>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
class Renderer {
public:
	Renderer();
	Renderer(int ancho, int alto);
	bool iniciarRenderer();
	void cerrarRenderer();
	SDL_Renderer* getRenderer() { return this->gRenderer; }
	void reset();
	
private:
	std::thread t_draw;
	SDL_Window* gWindow = NULL;
	SDL_Renderer* gRenderer = NULL;
	TTF_Font *gFont = NULL;
	int anchoVentana, altoVentana;
	void draw();
	bool rendererActivo;
};

#endif