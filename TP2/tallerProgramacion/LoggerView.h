#pragma once

#ifndef LOGGERVIEW_H
#define LOGGERVIEW_H

#include "Ltexture.h"
#include <string>
#include "Usuario.h"
#include "ManejadorInput.h"
using namespace std;

class LoggerView
{
public:
	LoggerView(SDL_Renderer* renderer);
	~LoggerView();
	bool init();
	void render();
	bool loadMedia();
	bool close();
	Usuario* showLogin();
	void update();
	Usuario* getUsuario();
private:
	TTF_Font *gFont = NULL;
	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 480;
	SDL_Color textColor;
	bool success;
	Usuario* usuario;
	SDL_Renderer* gRenderer = NULL;
	Ltexture* gPromptTextTexture;
	Ltexture* gInputTextTexture;
	Ltexture* gPromptPasswordTextTexture;
	Ltexture* gInputPasswordTextTexture;
	std::string inputText;
	bool isWrittingPassord = false;
	bool datosCargados = false;
};

#endif