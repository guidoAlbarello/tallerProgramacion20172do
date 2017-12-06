#include "Escenario.h"
#include <iostream>

Escenario::Escenario(Renderer* renderer) {
	this->renderer = renderer;
	this->nivel = 0;
	this->tablero = new TableroPuntosEnJuego(renderer->getRendererJuego());
	this->tablero->init();
}

Escenario::Escenario() {

}

void Escenario::iniciar() {
	// Se cargan los sprites propios del escenario aca y se agregan a la lista de sprites, en vez de que los tenga el escenario
	cielo = new Sprite();
	std::string skyFileName = "fondo/sky.png";
	cielo->setId("cielo");
	cielo->setPosicion(0, POS_Y_CIELO);
	posicionCielo.setX(0);
	posicionCielo.setY(POS_Y_CIELO);
	cielo->setAnchoYAlto(1600, ALTO_CIELO);
	cielo->load(skyFileName, this->renderer->getRendererJuego()); // Setear el id antes del load!

	colinas = new Sprite();
	std::string hillsFileName = "fondo/hills.png";
	colinas->setId("colinas");
	colinas->setPosicion(0, POS_Y_COLINAS);
	posicionColinas.setX(0);
	posicionColinas.setY(POS_Y_COLINAS);
	colinas->setAnchoYAlto(1600, ALTO_COLINAS);
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
	//	gPromptTextTexture->loadFromRenderedText(this->mensaje, textColor);
	//gPromptTextTexture->render(SCREEN_WIDTH - WIDTH, 0);
	if (limiteImagenCielo())
		posicionCielo.setX((int(posicionCielo.getX() + VELOCIDAD_CIELO * deltaTiempo)));
	else
		posicionCielo.setX(0);
	if (limiteImagenColinas())
		posicionColinas.setX((int(posicionColinas.getX() + VELOCIDAD_COLINAS * deltaTiempo)));
	else
		posicionColinas.setX(0);
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

void Escenario::setPosicionColinas(int x, int y) {
	while (x > 0) {
		x = x - 800;
	}
	while (x < -800) {
		x = x + 800;
	}
	this->posicionColinas.setX(x);
	this->posicionColinas.setY(y);
}

void Escenario::render() {
	//Falta la logica que hace funcionar la calesita
	//->cielo->setPosicion(posicionCielo.getX(), posicionCielo.getY());
	this->cielo->renderStaticEnCoord(renderer, posicionCielo.getX(), posicionCielo.getY());
	this->colinas->renderStaticEnCoord(renderer, posicionColinas.getX(), posicionColinas.getY());
	this->tablero->setVelocidad(velocidad);
	std::stringstream sTiempo;
	sTiempo << tiempo;
	this->tablero->setTiempo("Tiempo: " + sTiempo.str() + "s");
	this->tablero->setDistancia(distancia);
	this->tablero->update();
	//this->colinas->renderStatic(renderer);
	//Renderear puntos
}

void Escenario::setDistancia(long distancia) {
	this->distancia = distancia;
}

void Escenario::setVelocidad(long velocidad) {
	this->velocidad = velocidad;
}

void Escenario::setTiempo(long tiempo) {
	this->tiempo = tiempo;
}

void Escenario::cambiarNivel() {
	this->nivel++;
	cambiarCieloYColinas();
}

void Escenario::cambiarCieloYColinas() {
	std::string skyFileName;
	std::string hillsFileName;

	switch (this->nivel) {
	case 0:
		// dia
		cielo = new Sprite();
		skyFileName = "fondo/sky.png";
		cielo->setId("cielo");
		cielo->setPosicion(0, POS_Y_CIELO);
		posicionCielo.setX(0);
		posicionCielo.setY(POS_Y_CIELO);
		cielo->setAnchoYAlto(1600, ALTO_CIELO);
		cielo->load(skyFileName, this->renderer->getRendererJuego()); // Setear el id antes del load!

		colinas = new Sprite();
		hillsFileName = "fondo/hills.png";
		colinas->setId("colinas");
		colinas->setPosicion(0, POS_Y_COLINAS);
		posicionColinas.setX(0);
		posicionColinas.setY(POS_Y_COLINAS);
		colinas->setAnchoYAlto(1600, ALTO_COLINAS);
		colinas->load(hillsFileName, this->renderer->getRendererJuego()); // Setear el id antes del load!
		break;
	case 1:
		// tarde
		cielo = new Sprite();
		skyFileName = "fondo/sky_tarde.png";
		cielo->setId("cielo");
		cielo->setPosicion(0, POS_Y_CIELO);
		posicionCielo.setX(0);
		posicionCielo.setY(POS_Y_CIELO);
		cielo->setAnchoYAlto(1600, ALTO_CIELO);
		cielo->load(skyFileName, this->renderer->getRendererJuego()); // Setear el id antes del load!

		colinas = new Sprite();
		hillsFileName = "fondo/hills_tarde.png";
		colinas->setId("colinas");
		colinas->setPosicion(0, POS_Y_COLINAS);
		posicionColinas.setX(0);
		posicionColinas.setY(POS_Y_COLINAS);
		colinas->setAnchoYAlto(1600, ALTO_COLINAS);
		colinas->load(hillsFileName, this->renderer->getRendererJuego()); // Setear el id antes del load!
		break;
	case 2:
		// noche
		cielo = new Sprite();
		skyFileName = "fondo/sky_noche.png";
		cielo->setId("cielo");
		cielo->setPosicion(0, POS_Y_CIELO);
		posicionCielo.setX(0);
		posicionCielo.setY(POS_Y_CIELO);
		cielo->setAnchoYAlto(1600, ALTO_CIELO);
		cielo->load(skyFileName, this->renderer->getRendererJuego()); // Setear el id antes del load!

		colinas = new Sprite();
		hillsFileName = "fondo/hills_noche.png";
		colinas->setId("colinas");
		colinas->setPosicion(0, POS_Y_COLINAS);
		posicionColinas.setX(0);
		posicionColinas.setY(POS_Y_COLINAS);
		colinas->setAnchoYAlto(1600, ALTO_COLINAS);
		colinas->load(hillsFileName, this->renderer->getRendererJuego()); // Setear el id antes del load!
		break;
	}

}