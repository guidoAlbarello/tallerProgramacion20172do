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

void ManejadorDeTexturas::drawAnimatedSprite(std::string id, int x, int y, int ancho, int alto, int filaActual, int frameActual, int anchoPantalla, int altoPantalla, int zIndex, SDL_Renderer * pRenderer, SDL_RendererFlip flip) {
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
	destRect.w = ancho*3/2;
	srcRect.h = alto;
	destRect.h = alto*3/2;
	destRect.x = (x - destRect.w/2);
	destRect.y = (y - destRect.h/2);
	if (zIndex != 0) {
		destRect.x /= zIndex * FACTOR_PERSPECTIVA;
		destRect.y /= zIndex * FACTOR_PERSPECTIVA;
	}
	destRect.x += -camara->getPosicion()->getX() + anchoPantalla / 2;
	destRect.y += -camara->getPosicion()->getY() + altoPantalla * 3/ 4;
	SDL_RenderCopyEx(pRenderer, texturas[id], &srcRect, &destRect, 0, 0, flip);
}

void ManejadorDeTexturas::drawStaticSprite(std::string id, int x, int y, int ancho, int alto, int anchoPantalla, int zIndex, SDL_Renderer* pRenderer, SDL_RendererFlip flip) {
	SDL_Rect srcRect;
	SDL_Rect destRect;
	srcRect.x = 0;
	srcRect.y = 0;
	srcRect.w = destRect.w = ancho;
	srcRect.h = destRect.h = alto;
	destRect.x = (x - ancho / 2) - camara->getPosicion()->getX() + anchoPantalla / 2;
	destRect.y = (y - alto / 2);
	destRect.x /= zIndex * FACTOR_PERSPECTIVA;
	destRect.y /= zIndex * FACTOR_PERSPECTIVA;
	SDL_RenderCopyEx(pRenderer, texturas[id], &srcRect, &destRect, 0, 0, flip);
}


void ManejadorDeTexturas::dibujarSprite(std::string id, int x, int y, int ancho, int alto, int anchoPantalla, int zIndex, SDL_Renderer* pRenderer, SDL_RendererFlip flip) {
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

void ManejadorDeTexturas::dibujarTramo(int x, int y, int anchoPantalla, int altoPantalla, SDL_Renderer* renderer) {
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // the rect color (solid red)
	SDL_Rect tramo;
	tramo.x = x;
	tramo.y = y;
	tramo.h = 50;
	tramo.w = 200;
	if (y != 0) {
		tramo.x /= y * FACTOR_PERSPECTIVA;
		tramo.y /= y * FACTOR_PERSPECTIVA;
	}
	tramo.x += -camara->getPosicion()->getX() + anchoPantalla / 2;
	tramo.y += -camara->getPosicion()->getY() + altoPantalla * 3 / 4;
	SDL_RenderFillRect(renderer, &tramo);
}

ManejadorDeTexturas::~ManejadorDeTexturas() {
}
