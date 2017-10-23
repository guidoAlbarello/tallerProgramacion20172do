#include "Renderer.h"

Renderer::Renderer() {
	this->anchoVentana = 800;
	this->altoVentana = 600;
}

Renderer::Renderer(int ancho, int alto) {
	this->anchoVentana = ancho;
	this->altoVentana = alto;
}

bool Renderer::iniciarRenderer() {

	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	} else {
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
			printf("Warning: Linear texture filtering not enabled!");
		}

		//Create window
		gWindowJuego = SDL_CreateWindow("Outrun", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, anchoVentana, altoVentana, SDL_WINDOW_SHOWN);
		if (gWindowJuego == NULL) {
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		} else {
			printf("Window created");

			//Create vsynced renderer for window
			gRendererJuego = SDL_CreateRenderer(gWindowJuego, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (gRendererJuego == NULL) {
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			} else {
				printf("Renderer created");
				//Initialize renderer color
				SDL_SetRenderDrawColor(gRendererJuego, 0xFF, 0xFF, 0xFF, 0xFF);

				//Initialize PNG loading

				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags)) {
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}

				//Initialize SDL_ttf

				if (TTF_Init() == -1) {
					printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
					success = false;
				}

			}
		}


		//Create mapa window
		this->gWindowMapa = SDL_CreateWindow("Circuito", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, anchoVentana, altoVentana, SDL_WINDOW_SHOWN);
		if (gWindowMapa == NULL) {
			printf("Window mapa could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else {
			printf("Window created");

			//Create vsynced renderer for window
			this->gRendererMapa = SDL_CreateRenderer(gWindowMapa, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (gRendererMapa == NULL) {
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else {
				printf("Renderer created");
				//Initialize renderer color
				SDL_SetRenderDrawColor(gRendererMapa, 242, 242, 242, 255);

				//Initialize PNG loading
				/*int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags)) {
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}*/

				//Initialize SDL_ttf
				/*if (TTF_Init() == -1) {
					printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
					success = false;
				}*/
			}
		}
	}

	return success;
}

void Renderer::cerrarRenderer() {
	//Free global font
	
	//Destroy window	
	SDL_DestroyRenderer(gRendererJuego);
	SDL_DestroyWindow(gWindowJuego);
	gWindowJuego = NULL;
	gRendererJuego = NULL;

	//Quit SDL subsystems
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

void Renderer::reset() {
	SDL_RenderPresent(gRendererJuego);
}

