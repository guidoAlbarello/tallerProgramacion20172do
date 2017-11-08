#include "Escenario.h"
#include <iostream>

Escenario::Escenario(Renderer* renderer) {
	this->renderer = renderer;
}

Escenario::Escenario() {

}

void Escenario::iniciar() {
	// Se cargan los sprites propios del escenario aca y se agregan a la lista de sprites, en vez de que los tenga el escenario
	cielo = new Sprite();
	std::string skyFileName = "fondo/sky.png";
	cielo->setId("cielo");
	cielo->setPosicion(0, 0);
	posicionCielo.setX(0);
	posicionCielo.setY(0);
	cielo->setAnchoYAlto(1600, 400);
	cielo->load(skyFileName, this->renderer->getRendererJuego()); // Setear el id antes del load!

	colinas = new Sprite();
	std::string hillsFileName = "fondo/hills.png";
	colinas->setId("colinas");
	colinas->setPosicion(0, 100);
	posicionColinas.setX(0);
	posicionColinas.setY(100);
	colinas->setAnchoYAlto(800, 300);
	colinas->load(hillsFileName, this->renderer->getRendererJuego()); // Setear el id antes del load!
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
	if (limiteImagenCielo())
		posicionCielo.setX((int(posicionCielo.getX() + VELOCIDAD_CIELO * deltaTiempo)));
	else
		posicionCielo.setX(0);
	//posicionColinas.setX((int(posicionColinas.getX() + VELOCIDAD_COLINAS * deltaTiempo)) % colinas->getAncho());
}
void Escenario::setPosicionCielo(int x, int y){
	
	while (x > 0) {
		x = x - 800;
	}
	while (x < -800) {
		x = x + 800;
	}
	this->posicionCielo.setX(x);
	this->posicionCielo.setY(y);
}
void Escenario::render() {
	//Falta la logica que hace funcionar la calesita
	//->cielo->setPosicion(posicionCielo.getX(), posicionCielo.getY());
	this->cielo->renderStaticEnCoord(renderer, posicionCielo.getX(), posicionCielo.getY());
	this->colinas->renderStatic(renderer);
}
