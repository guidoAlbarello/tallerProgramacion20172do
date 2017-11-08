#include "Sprite.h"

void Sprite::renderStatic(Renderer * renderer) {
	ManejadorDeTexturas::getInstance()->dibujarSprite(id, posicion.getX(), posicion.getY(), ancho, alto, 
		renderer->getAnchoVentana(), renderer->getRendererJuego(), SDL_FLIP_NONE);
}
void Sprite::renderStaticEnCoord(Renderer * renderer, int xDest, int yDest) {
	ManejadorDeTexturas::getInstance()->dibujarSpriteEnCoord(id, posicion.getX(), posicion.getY(), ancho, alto,
		ancho, renderer->getRendererJuego(), SDL_FLIP_NONE, xDest, yDest);
}
// Setear el id antes del load!
void Sprite::load(std::string fileName, SDL_Renderer * pRenderer) {
	ManejadorDeTexturas::getInstance()->load(fileName, this->id, pRenderer);
}

void Sprite::render(Renderer* renderer) {
	ManejadorDeTexturas::getInstance()->drawAnimatedSprite(id, x, y, ancho, alto, filaActual, frameActual,
		renderer->getAnchoVentana(), renderer->getAltoVentana(), renderer->getRendererJuego(), SDL_FLIP_NONE, grisarSprite);
}
