#include "Renderer.h"

Renderer::Renderer() {
	this->rendererActivo = true;
}

bool Renderer::iniciarRenderer() {
	return false; //todo el sdl para levantar la ventna this->t_draw = std::thread(&Renderer::draw, this);
}

void Renderer::draw() {
	while (rendererActivo) {

	}
}
