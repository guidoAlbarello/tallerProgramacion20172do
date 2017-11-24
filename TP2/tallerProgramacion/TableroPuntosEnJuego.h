#pragma once

#ifndef TABLEROPUNTOSENJUEGO_H
#define TABLEROPUNTOSENJUEGO_H

#include "Ltexture.h"
#include <string>
#include "Logger.h"
#include "Constantes.h"

using namespace std;

class TableroPuntosEnJuego
{
public:
	TableroPuntosEnJuego(SDL_Renderer* renderer);
	~TableroPuntosEnJuego();

	void init();
	void update();
	void setDistancia(string distancia);
	void setTiempo(string tiempo);
	void setVelocidad(string velocidad);
private:
	const int WIDTH = 300;
	const int HEIGHT = 150;
	const int SCREEN_WIDTH = 800;
	SDL_Color textColor;
	Ltexture* gPromptDistancia;
	Ltexture* gPromptTiempo;
	Ltexture* gPromptVelocidad;
	string distancia;
	string tiempo;
	string velocidad;
	bool success;
	SDL_Renderer* gRenderer = NULL;

};
#endif

