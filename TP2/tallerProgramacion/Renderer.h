#pragma once

#ifndef RENDERER_H
#define RENDERER_H
#include <thread>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "Logger.h"

class Renderer {
public:
	Renderer();
	Renderer(int ancho, int alto);
	bool iniciarRendererJuego();
	bool iniciarRendererMapa();
	void cerrarRenderer();
	SDL_Renderer* getRendererJuego() { return this->gRendererJuego; }
	SDL_Renderer* getRendererMapa() { return this->gRendererMapa; }
	SDL_Window* getWindowJuego() { return this->gWindowJuego; }
	SDL_Window* getWindowMapa() { return this->gWindowMapa; }
	void reset();
	int getAnchoVentana() { return this->anchoVentana; }
	int getAltoVentana() { return this->altoVentana; }
	void setAnchoYAlto(int ancho, int alto) { anchoVentana = ancho; altoVentana = alto; }
private:
	SDL_Window* gWindowJuego = NULL;
	SDL_Window* gWindowMapa = NULL;
	SDL_Renderer* gRendererJuego = NULL;
	SDL_Renderer* gRendererMapa = NULL;
	TTF_Font *gFont = NULL;
	int anchoVentana, altoVentana;
};

#endif