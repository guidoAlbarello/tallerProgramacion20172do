#include "Sprite.h"

void Sprite::renderStatic(Renderer * renderer) {
	ManejadorDeTexturas::getInstance()->drawStaticSprite(id, posicion.getX(), posicion.getY(), ancho, alto, 
		renderer->getAnchoVentana(), zIndex, renderer->getRendererJuego(), SDL_FLIP_NONE);
}

// Setear el id antes del load!
void Sprite::load(std::string fileName, SDL_Renderer * pRenderer) {
	ManejadorDeTexturas::getInstance()->load(fileName, this->id, pRenderer);
}

void Sprite::render(Renderer* renderer) {
	ManejadorDeTexturas::getInstance()->dibujarSprite(id, posicion.getX(), posicion.getY(), ancho, alto,
		renderer->getAnchoVentana(), zIndex, renderer->getRendererJuego(), SDL_FLIP_NONE);
	// Por el momento se renderean con este metodo hasta revisar bien este
	/*ManejadorDeTexturas::getInstance()->drawAnimatedSprite(id, posicion.getX(), posicion.getY(), ancho, alto, filaActual,
			frameActual, renderer->getAnchoVentana(), posicion.getY(), renderer->getRendererJuego(), SDL_FLIP_NONE);*/
}
