#include "ManejadorDeTexturas.h"

ManejadorDeTexturas* ManejadorDeTexturas::instance = 0;

ManejadorDeTexturas * ManejadorDeTexturas::getInstance() {
	if (!instance) {
		instance = new ManejadorDeTexturas();
	}

	return instance;
}



ManejadorDeTexturas::ManejadorDeTexturas() {
	this->camara = new Camara();
}

void ManejadorDeTexturas::drawAnimatedSprite(std::string id, int x, int y, int ancho, int alto, int filaActual, int frameActual, int anchoPantalla, int altoPantalla, SDL_Renderer * pRenderer, SDL_RendererFlip flip) {
	float zIndex = abs(camara->getPosicion()->getY() - 1 - y);		//hacer y - h/2 en vez de - y!!!!!!!!!!
	SDL_Rect srcRect;
	SDL_Rect destRect;
	srcRect.x = ancho * frameActual;
	srcRect.y = alto * (filaActual - 1);
	//srcRect.w = destRect.w = ancho;
	//srcRect.h = destRect.h = alto;
	//destRect.x = (x - ancho / 2);
	//destRect.y = (y - alto / 2);
	//esti para ver el auto al doble de grande
	srcRect.w = ancho;
	destRect.w = ancho * 3 / 2;
	srcRect.h = alto;
	destRect.h = alto * 3 / 2;
	destRect.x = (x - destRect.w / 2);
	destRect.y = -(y - destRect.h / 2 - camara->getPosicion()->getY() - 1);
	if (destRect.y != 0) {
		//destRect.y *= FACTOR_PERSPECTIVA;//agregar h y w 
	}
	destRect.x += -camara->getPosicion()->getX() + anchoPantalla / 2;
	destRect.y += altoPantalla * 3 / 4;
	SDL_RenderCopyEx(pRenderer, texturas[id], &srcRect, &destRect, 0, 0, flip);
}

void ManejadorDeTexturas::drawStaticSprite(std::string id, int x, int y, int ancho, int alto, int anchoPantalla, SDL_Renderer* pRenderer, SDL_RendererFlip flip) {
	float zIndex = camara->getPosicion()->getY() - 1 - y - alto / 2;
	SDL_Rect srcRect;
	SDL_Rect destRect;
	srcRect.x = 0;
	srcRect.y = 0;
	srcRect.w = destRect.w = ancho;
	srcRect.h = destRect.h = alto;
	destRect.x = (x - ancho / 2) - camara->getPosicion()->getX() + anchoPantalla / 2;
	destRect.y = (y - alto / 2);

	SDL_RenderCopyEx(pRenderer, texturas[id], &srcRect, &destRect, 0, 0, flip);
}


void ManejadorDeTexturas::dibujarSprite(std::string id, int x, int y, int ancho, int alto, int anchoPantalla, SDL_Renderer* pRenderer, SDL_RendererFlip flip) {
	SDL_Rect destRect;
	destRect.x = x;
	destRect.y = y;
	destRect.w = ancho;
	destRect.h = alto;
	SDL_RenderCopyEx(pRenderer, texturas[id], NULL, &destRect, 0, 0, flip);
}

bool ManejadorDeTexturas::load(std::string fileName, std::string id, SDL_Renderer * pRenderer) {
	SDL_Surface* pTempSurface = IMG_Load(fileName.c_str());
	if (pTempSurface == 0) {
		return false;
	}
	SDL_Texture* pTexture =
		SDL_CreateTextureFromSurface(pRenderer, pTempSurface);
	SDL_FreeSurface(pTempSurface);
	if (pTexture != 0) {
		texturas[id] = pTexture;
		return true;
	}
	return false;
}

void ManejadorDeTexturas::dibujarTramo(int x, int y, int ancho, int alto, int anchoPantalla, int altoPantalla, SDL_Renderer* renderer, bool grisOscuro) {
	if (grisOscuro)
		SDL_SetRenderDrawColor(renderer, 125, 125, 125, 255);
	else
		SDL_SetRenderDrawColor(renderer, 175, 175, 175, 255);

	float zIndex = camara->getPosicion()->getY() - 1 - y - alto / 2;
	SDL_Rect tramo;


	tramo.y = (y - alto / 2 - camara->getPosicion()->getY() - 1);
	tramo.h = alto;
	tramo.w = ancho;
	if (tramo.y != 0) {
		//tramo.h *= normZIndex(y);
		tramo.w /= normZIndex(y) * 3;
	}
	tramo.x = x - tramo.w / 2;
	tramo.x = tramo.x + anchoPantalla / 2 - camara->getPosicion()->getX();
	tramo.y = -tramo.y + altoPantalla * 3 / 4;
	SDL_RenderFillRect(renderer, &tramo);
}

float ManejadorDeTexturas::normZIndex(float zIndex) {
	float normalizado;

	if (zIndex > Z_FAR + camara->getPosicion()->getY())
		normalizado = 0;
	else
		if (zIndex < Z_NEAR + camara->getPosicion()->getY())
			normalizado = (zIndex / camara->getPosicion()->getY()) / Z_FAR;
		else
			normalizado = (zIndex - Z_NEAR - camara->getPosicion()->getY()) / Z_FAR;

	return abs(normalizado);
}

ManejadorDeTexturas::~ManejadorDeTexturas() {
}
