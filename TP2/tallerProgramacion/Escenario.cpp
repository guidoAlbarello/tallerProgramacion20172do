#include "Escenario.h"
#include <iostream>

Escenario::Escenario(Renderer* renderer) {
	cielo = new Ltexture(renderer->getRendererJuego());
	colinas = new Ltexture(renderer->getRendererJuego());
}

void Escenario::iniciar() {
	//renderear fondo... todo hardcodeado
	std::cout << "iniciando escenario" << std::endl;
	if (cielo->loadFromFile("fondo/sky.png")) {
		std::cout << "levanto bien el asset del cielo" << std::endl;
		cielo->render(0, 0);
	}
	else {
		std::cout << "levanto el asset del cielo" << std::endl;
	}
	if (colinas->loadFromFile("fondo/hills.png")) {
		std::cout << "levanto bien el asset de las colinas" << std::endl;
		colinas->render(0, 120);
		//120 porque el asset mide 480, entonces 480 + 120 = 600
	}
	else {
		std::cout << "levanto mal el asset de las colinas" << std::endl;
	}
}

Escenario::~Escenario() {
	if (cielo != NULL) {
		cielo->free();
	}
	if (colinas != NULL) {
		colinas->free();
	}
}