#include "Servidor.h"
#include "Cliente.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <sstream>
#include "LoggerView.h"
#include "Ltexture.h"

using namespace std;

#define ES_SERVIDOR false
//Screen dimension constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

//Texture wrapper class

bool init();

//Loads media
//bool loadMedia();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Globally used font
TTF_Font *gFont = NULL;

//Scene textures
//Ltexture* gPromptTextTexture;
//Ltexture* gInputTextTexture;
//      Ltexture gPromptPasswordTextTexture;
//      Ltexture gInputPasswordTextTexture;
bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		int imgFlags = IMG_INIT_PNG;
		if (!(IMG_Init(imgFlags) & imgFlags))
		{
			printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
			success = false;
		}

		//Initialize SDL_ttf

		if (TTF_Init() == -1)
		{
			printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
			success = false;
		}

	}
	return success;
	

}

void close()
{
	
	//Free global font


	//Destroy window	
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

int main(int argc, char* argv[]) {

	printf("Inicializando");
	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		//LoggerView loggerView = LoggerView(gRenderer);
		//printf("Antes del showlogin");
		//loggerView.showLogin();

		Cliente::getInstance()->iniciarCliente();
		//break;


	}

	//Free resources and close SDL
	close();
	system("PAUSE");
	return 0;
}