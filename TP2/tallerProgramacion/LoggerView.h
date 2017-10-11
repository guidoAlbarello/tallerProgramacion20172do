#pragma once

#ifndef LOGGERVIEW_H
#define LOGGERVIEW_H

#include "Ltexture.h"
#include <string>

using namespace std;

class LoggerView
{
public:
	LoggerView(SDL_Renderer* renderer);
	~LoggerView();

	bool loadMedia();
	void showLogin();

private:
	TTF_Font *gFont = NULL;
	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 480;

	SDL_Renderer* gRenderer = NULL;
	Ltexture* gPromptTextTexture;
	Ltexture* gInputTextTexture;
	Ltexture* gPromptPasswordTextTexture;
	Ltexture* gInputPasswordTextTexture;
};

#endif