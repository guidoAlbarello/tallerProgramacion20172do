#include "Renderer.h"

Renderer::Renderer() {
	this->anchoVentana = 800;
	this->altoVentana = 600;
}

Renderer::Renderer(int ancho, int alto) {
	this->anchoVentana = ancho;
	this->altoVentana = alto;
}

bool Renderer::iniciarRendererJuego() {

	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		Logger::getInstance()->log(Error, "No se pudo inicializar SDL");
		success = false;
	} else {
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
			Logger::getInstance()->log(Debug, "Warning: Linear texture filtering not enabled!");
		}

		//Create window
		gWindowJuego = SDL_CreateWindow("Outrun", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, anchoVentana, altoVentana, SDL_WINDOW_SHOWN);
		if (gWindowJuego == NULL) {
			Logger::getInstance()->log(Error, "Window juego could not be created!");
			success = false;
		} else {
			Logger::getInstance()->log(Debug, "Window juego created");
			//Create vsynced renderer for window
			gRendererJuego = SDL_CreateRenderer(gWindowJuego, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (gRendererJuego == NULL) {
				Logger::getInstance()->log(Error, "Renderer juego could not be created!");
				success = false;
			} else {
				Logger::getInstance()->log(Debug, "Renderer juego created");
				//Initialize renderer color
				SDL_SetRenderDrawColor(gRendererJuego, 0xFF, 0xFF, 0xFF, 0xFF);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags)) {
					Logger::getInstance()->log(Error, "SDL_image could not initialize!");
					success = false;
				}

				//Initialize SDL_ttf
				if (TTF_Init() == -1) {
					Logger::getInstance()->log(Error, "SDL_ttf could not initialize!");
					success = false;
				}
			}
		}
	}
	return success;
}

bool Renderer::iniciarRendererMapa() {

	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		Logger::getInstance()->log(Error, "No se pudo inicializar SDL");
		success = false;
	}
	else {
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
			Logger::getInstance()->log(Debug, "Warning: Linear texture filtering not enabled!");
		}

		//Create mapa window
		this->gWindowMapa = SDL_CreateWindow("Circuito", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, anchoVentana, altoVentana, SDL_WINDOW_SHOWN);
		if (gWindowMapa == NULL) {
			Logger::getInstance()->log(Error, "Window mapa could not be created!");
			success = false;
		}
		else {
			Logger::getInstance()->log(Debug, "Window mapa created!");
			//Create vsynced renderer for window
			this->gRendererMapa = SDL_CreateRenderer(gWindowMapa, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (gRendererMapa == NULL) {
				Logger::getInstance()->log(Error, "Renderer mapa could not be created!");
				success = false;
			}
			else {
				Logger::getInstance()->log(Debug, "Renderer mapa created!");
				SDL_SetRenderDrawColor(gRendererMapa, 242, 242, 242, 255); // Initialize renderer color
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

