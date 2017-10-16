#include "Sprite.h"

void Sprite::render(Renderer* renderer) {
	ManejadorDeTexturas::getInstance()->drawAnimatedSprite(id, posicion.getX(), posicion.getY(), ancho, alto, filaActual,
			frameActual, renderer->getAnchoVentana(), zIndex, renderer->getRenderer(), SDL_FLIP_NONE);
}
