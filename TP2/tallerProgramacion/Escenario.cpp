#include "Escenario.h"
#include <iostream>

Escenario::Escenario(Renderer* renderer) {
	texture = new Ltexture(renderer->getRenderer());
}

void Escenario::iniciar() {
	//renderear fondo... todo hardcodeado
	std::cout << "iniciando escenario" << std::endl;
	if (texture->loadFromFile("fondo/sky.png")) {
		std::cout << "levanto bien" << std::endl;
		texture->render(0, 0);
	}
	else {
		std::cout << "levanto mal" << std::endl;
	}
}

Escenario::~Escenario() {
	if (texture != NULL) {
		texture->free();
	}
}