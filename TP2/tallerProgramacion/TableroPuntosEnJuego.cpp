#include "TableroPuntosEnJuego.h"



TableroPuntosEnJuego::TableroPuntosEnJuego(SDL_Renderer* renderer)
{
	this->gRenderer = renderer;
	this->gPromptDistancia = new Ltexture(gRenderer);
	this->gPromptVelocidad = new Ltexture(gRenderer);
	this->gPromptTiempo = new Ltexture(gRenderer);
}


TableroPuntosEnJuego::~TableroPuntosEnJuego()
{
	if (gPromptDistancia != NULL) {
		gPromptDistancia->free();
	}
	if (gPromptTiempo != NULL) {
		gPromptTiempo->free();
	}
	if (gPromptVelocidad != NULL) {
		gPromptVelocidad->free();
	}
}

void TableroPuntosEnJuego::setDistancia(long distancia) {
	this->distancia = distancia;
}

void TableroPuntosEnJuego::setTiempo(string tiempo) {
	this->tiempo = tiempo;
}
void TableroPuntosEnJuego::setVelocidad(long velocidad) {
	this->velocidad = velocidad;
}
void TableroPuntosEnJuego::init() {
	textColor = { 0, 0, 0, 0xFF };
	/*
	if (!gPromptTextTexture->loadFromRenderedText(this->mensaje, textColor)) {
		Logger::getInstance()->log(Error, "Ocurrio un error al iniciar la textura Usuario:");
		success = false;
	}
	*/

}

void TableroPuntosEnJuego::update() {
	int velocidadEnMs = this->velocidad * Constantes::FPS / ALTO_TRAMO;
	int velocidadEnKmh = velocidadEnMs * 3600 / 1000;
	string strVelocidadEnKmh = to_string(velocidadEnKmh);
	gPromptVelocidad->loadFromRenderedText("Velocidad: " + strVelocidadEnKmh + "km/h", textColor);
	gPromptVelocidad->render(10, 10);

	gPromptTiempo->loadFromRenderedText(this->tiempo, textColor);
	gPromptTiempo->render(310, 10);

	float distanciaEnKm = distancia / 1000.000;
	stringstream stream;
	stream << fixed << setprecision(3) << distanciaEnKm;
	string strDistancia = stream.str();
	gPromptDistancia->loadFromRenderedText("Distancia: " + strDistancia + "km", textColor);
	gPromptDistancia->render(510, 10);
}