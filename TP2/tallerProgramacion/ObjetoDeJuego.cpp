#include "ObjetoDeJuego.h"

ObjetoDeJuego::ObjetoDeJuego() {

}
ObjetoDeJuego::ObjetoDeJuego(SDL_Renderer* renderer) {
	this->texture == new Ltexture(renderer);
}

ObjetoDeJuego::~ObjetoDeJuego() {
	if (texture != NULL) {
		texture->free();
	}
}

void ObjetoDeJuego::update(Unidad delta) {
}

void ObjetoDeJuego::leerEntrada() {
}

