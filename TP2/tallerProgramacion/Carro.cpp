#include "Carro.h"

Carro::Carro(SDL_Renderer* renderer) : ObjetoDeJuego(renderer) {
	this->renderer = renderer;
}

void Carro::update(Unidad delta) {
	texture->render(CARRO_X, CARRO_Y);
}