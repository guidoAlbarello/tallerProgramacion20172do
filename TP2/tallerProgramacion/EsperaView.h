#pragma once

#ifndef ESPERA_VIEW_H
#define ESPERA_VIEW_H
#include"EstadoJuego.h"
#include "Ltexture.h"


class EsperaView
{
public:
	EsperaView(SDL_Renderer* renderer);
	~EsperaView();
	bool init();
	void render();
	bool loadMedia();
	bool close();
	void update();
private:
	TTF_Font *gFont = NULL;
	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 480;
	SDL_Color textColor;
	SDL_Renderer* gRenderer = NULL;
	Ltexture* gResutladoLoginTexture;
};

#endif