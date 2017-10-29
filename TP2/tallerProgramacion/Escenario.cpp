#include "Escenario.h"
#include <iostream>

Escenario::Escenario(Renderer* renderer) {
	this->renderer = renderer;
}

Escenario::Escenario() {

}

void Escenario::iniciar() {
	
	//this->cielo->load("fondo/sky.png", renderer->getRendererJuego());
	//this->colinas->load("fondo/hills.png", renderer->getRendererJuego());
	
	/*//renderear fondo... todo hardcodeado
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
	}*/
}

Escenario::~Escenario() {
	if (cielo != NULL) {
		delete cielo;
	}
	if (colinas != NULL) {
		delete colinas;
	}
}

void Escenario::update(Unidad deltaTiempo) {
	posicionCielo.setX( posicionCielo.getX() + VELOCIDAD_CIELO * deltaTiempo);
	posicionColinas.setX(posicionColinas.getX() + VELOCIDAD_COLINAS * deltaTiempo);
}

void Escenario::render() {
	//this->colinas->render(renderer);
	//this->cielo->render(renderer);
}
