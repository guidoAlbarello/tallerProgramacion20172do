#include "ObjetoDeJuego.h"

ObjetoDeJuego::ObjetoDeJuego() {
	this->texture = NULL;
}
ObjetoDeJuego::ObjetoDeJuego(SDL_Renderer* renderer) {
	this->texture = new Ltexture(renderer);
}

ObjetoDeJuego::~ObjetoDeJuego() {
	if (texture != NULL) {
		texture->free();
	}
}

void ObjetoDeJuego::update(Unidad delta) {
	//texture->render(0, 0);
}

void ObjetoDeJuego::leerEntrada() {
}

void ObjetoDeJuego::inicializar(string path) {
	//this->texture->loadFromFile(path);
}
