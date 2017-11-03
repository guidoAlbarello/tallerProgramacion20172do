#include "Sprite.h"

void Sprite::renderStatic(Renderer * renderer) {
	ManejadorDeTexturas::getInstance()->dibujarSprite(id, posicion.getX(), posicion.getY(), ancho, alto, 
		renderer->getAnchoVentana(), renderer->getRendererJuego(), SDL_FLIP_NONE);
}

// Setear el id antes del load!
void Sprite::load(std::string fileName, SDL_Renderer * pRenderer) {
	ManejadorDeTexturas::getInstance()->load(fileName, this->id, pRenderer);
}

void Sprite::render(Renderer* renderer) {
	ManejadorDeTexturas::getInstance()->drawAnimatedSprite(id, x, y, ancho, alto, filaActual, frameActual,
		renderer->getAnchoVentana(), renderer->getAltoVentana(), renderer->getRendererJuego(), SDL_FLIP_NONE, grisarSprite);
}
