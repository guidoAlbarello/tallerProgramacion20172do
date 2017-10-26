#include "Carro.h"

Carro::Carro(SDL_Renderer* renderer) : ObjetoDeJuego(renderer) {
	this->renderer = renderer;
	this->estado = DERECHO;
	this->texture = NULL;
}

void Carro::update(Unidad delta) {
	string path = "";
	switch (this->estado) {
	case DERECHO:
		path = PATH_DERECHO;
		break;
	case GIRANDO_DERECHA:
		path = PATH_GIRANDO_DERECHA;
		break;
	case GIRANDO_IZQUIERDA:
		path = PATH_GIRANDO_IZUIERDA;
		break;
	default:
		path = PATH_DERECHO;
		break;
	}
	
	texture->loadFromFile(path);
	texture->render(CARRO_X, CARRO_Y);
}

void Carro::setEstado(int estado) {
	this->estado = estado;
}

int Carro::getEstado() {
	return this->estado;
}