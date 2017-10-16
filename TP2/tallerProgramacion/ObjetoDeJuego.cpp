#include "ObjetoDeJuego.h"

ObjetoDeJuego::ObjetoDeJuego() {

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
	texture->render(0, 0);
}

void ObjetoDeJuego::leerEntrada(std::vector<SDL_Keycode> entradas) {

}

void ObjetoDeJuego::leerEntrada() {
}

void ObjetoDeJuego::inicializar(string path) {
	this->texture->loadFromFile(path);
}
