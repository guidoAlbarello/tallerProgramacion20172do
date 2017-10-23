#pragma once

#ifndef ESPERA_VIEW_H
#define ESPERA_VIEW_H
#include"EstadoJuego.h"
#include "Ltexture.h"


class EsperaView
{
public:
	EsperaView();
	~EsperaView();
	bool init();
	void render();
	bool loadMedia();
	bool close();
	void update();
	//Usuario* showLogin();
	//Usuario* getUsuario();
private:
	TTF_Font *gFont = NULL;
	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 480;
	SDL_Color textColor;
//	bool success;
//	Usuario* usuario;
	SDL_Renderer* gRenderer = NULL;
	Ltexture* gResutladoLoginTexture;
//	Ltexture* gInputTextTexture;
//	Ltexture* gPromptPasswordTextTexture;
//	Ltexture* gInputPasswordTextTexture;
//	std::string inputText;
//	bool datosCargados = false;
};

#endif