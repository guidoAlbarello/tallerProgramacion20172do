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

void TableroPuntosEnJuego::setDistancia(string distancia) {
	this->distancia = distancia;
}

void TableroPuntosEnJuego::setTiempo(string tiempo) {
	this->tiempo = tiempo;
}
void TableroPuntosEnJuego::setVelocidad(string velocidad) {
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

void  TableroPuntosEnJuego::update() {
	gPromptVelocidad->loadFromRenderedText(this->velocidad, textColor);
	gPromptVelocidad->render(400, 0);
	gPromptDistancia->loadFromRenderedText(this->distancia, textColor);
	gPromptDistancia->render(400, 40);
	gPromptTiempo->loadFromRenderedText(this->tiempo, textColor);
	gPromptTiempo->render(400, 80);

}