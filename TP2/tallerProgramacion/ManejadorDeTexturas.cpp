#include "ManejadorDeTexturas.h"

ManejadorDeTexturas* ManejadorDeTexturas::instance = 0;

ManejadorDeTexturas * ManejadorDeTexturas::getInstance() {
	if (!instance) {
		instance = new ManejadorDeTexturas();
	}

	return instance;
}

ManejadorDeTexturas::ManejadorDeTexturas() {

}

void ManejadorDeTexturas::drawAnimatedSprite(std::string id, int x, int y, int ancho, int alto, int filaActual, int frameActual, int posicionCamara, int anchoPantalla, int zIndex, SDL_Renderer * pRenderer, SDL_RendererFlip flip) {
	SDL_Rect srcRect;
	SDL_Rect destRect;
	srcRect.x = ancho * frameActual;
	srcRect.y = alto * (filaActual - 1);
	srcRect.w = destRect.w = ancho;
	srcRect.h = destRect.h = alto;
	destRect.x = (x - ancho / 2) - posicionCamara + anchoPantalla / 2;
	destRect.y = (y - alto / 2);
	destRect.x /= zIndex * FACTOR_PERSPECTIVA;
	destRect.y /= zIndex * FACTOR_PERSPECTIVA;
	SDL_RenderCopyEx(pRenderer, texturas[id], &srcRect, &destRect, 0, 0, flip);
}

void ManejadorDeTexturas::drawStaticSprite(std::string id, int x, int y, int ancho, int alto, int posicionCamara, int anchoPantalla, int zIndex, SDL_Renderer* pRenderer, SDL_RendererFlip flip) {
	SDL_Rect srcRect;
	SDL_Rect destRect;
	srcRect.x = 0;
	srcRect.y = 0;
	srcRect.w = destRect.w = ancho;
	srcRect.h = destRect.h = alto;
	destRect.x = (x - ancho / 2) - posicionCamara + anchoPantalla / 2;
	destRect.y = (y - alto / 2);
	destRect.x /= zIndex * FACTOR_PERSPECTIVA;
	destRect.y /= zIndex * FACTOR_PERSPECTIVA;
	SDL_RenderCopyEx(pRenderer, texturas[id], &srcRect, &destRect, 0, 0, flip);
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

ManejadorDeTexturas::~ManejadorDeTexturas() {
}
