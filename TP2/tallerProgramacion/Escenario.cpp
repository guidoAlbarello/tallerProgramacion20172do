#include "Escenario.h"
#include <iostream>

Escenario::Escenario(Renderer* renderer) {
	cielo = new Ltexture(renderer->getRenderer());
	colinas = new Ltexture(renderer->getRenderer());
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
void Escenario::update(Unidad delta) {
	//pregunto x el estado
	//en base a eso... updateo la posicion a renderear
	if (cielo->loadFromFile("fondo/sky.png")) {
		cielo->render(0, 0);
	}
	else {
		std::cout << "levanto mal el asset del cielo" << std::endl;
	}
	if (colinas->loadFromFile("fondo/hills.png")) {
		colinas->render(0, 120);
		//120 porque el asset mide 480, entonces 480 + 120 = 600
	}
	else {
		std::cout << "levanto mal el asset de las colinas" << std::endl;
	}
}

void Escenario::leerEntrada(std::vector<SDL_Keycode> entradas) {
	//recibo una copia....
}