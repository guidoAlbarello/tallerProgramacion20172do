#pragma once

#ifndef TABLEROPUNTOSENJUEGO_H
#define TABLEROPUNTOSENJUEGO_H

#include "Ltexture.h"
#include <string>
#include "Logger.h"
#include "Constantes.h"
#include "MapaView.h"
#include <iomanip> // setprecision
#include <sstream> // stringstream

using namespace std;

class TableroPuntosEnJuego
{
public:
	TableroPuntosEnJuego(SDL_Renderer* renderer);
	~TableroPuntosEnJuego();

	void init();
	void update();
	void setDistancia(long distancia);
	void setTiempo(string tiempo);
	void setVelocidad(long velocidad);
private:
	const int WIDTH = 300;
	const int HEIGHT = 150;
	const int SCREEN_WIDTH = 800;
	SDL_Color textColor;
	Ltexture* gPromptDistancia;
	Ltexture* gPromptTiempo;
	Ltexture* gPromptVelocidad;
	long distancia;
	string tiempo;
	long velocidad;
	bool success;
	SDL_Renderer* gRenderer = NULL;

};
#endif

