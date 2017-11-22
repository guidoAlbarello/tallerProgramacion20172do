#include "TableroPuntosEnJuego.h"



TableroPuntosEnJuego::TableroPuntosEnJuego(SDL_Renderer* renderer)
{
	this->gRenderer = renderer;
	this->gPromptTextTexture = new Ltexture(gRenderer);
	this->mensaje = "Puntos:";
	this->gPromptTiempo = new Ltexture(gRenderer);
}


TableroPuntosEnJuego::~TableroPuntosEnJuego()
{
	if (gPromptTextTexture != NULL) {
		gPromptTextTexture->free();
	}
}

void TableroPuntosEnJuego::setMensaje(string mensaje) {
	this->mensaje = mensaje;
}

void TableroPuntosEnJuego::setTiempo(string tiempo) {
	this->tiempo = tiempo;
}

void TableroPuntosEnJuego::init() {
	textColor = { 0, 0, 0, 0xFF };
	
	if (!gPromptTextTexture->loadFromRenderedText(this->mensaje, textColor)) {
		Logger::getInstance()->log(Error, "Ocurrio un error al iniciar la textura Usuario:");
		success = false;
	}
	

}

void  TableroPuntosEnJuego::update() {
	gPromptTextTexture->loadFromRenderedText(this->mensaje, textColor);
	gPromptTextTexture->render(SCREEN_WIDTH - WIDTH, 0);
	gPromptTiempo->loadFromRenderedText(tiempo, textColor);
	gPromptTiempo->render(400, 40);
}