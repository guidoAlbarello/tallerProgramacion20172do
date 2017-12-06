#include "EsperaView.h"

EsperaView::EsperaView(SDL_Renderer* renderer) {
	this->gRenderer = renderer;
	this->gResutladoLoginTexture = new Ltexture(gRenderer);
	this->gBackgroundImage = new Ltexture(gRenderer);
	rectanguloFullscreen = new SDL_Rect();
}

EsperaView::~EsperaView() {
	if (gResutladoLoginTexture != NULL) {
		gResutladoLoginTexture->free();
	}
	if (gBackgroundImage != NULL) {
		gBackgroundImage->free();
	}
	if(rectanguloFullscreen != NULL)
		delete rectanguloFullscreen;
	TTF_CloseFont(gFont);
	gFont = NULL;
}

bool EsperaView::init() {
	textColor = { 0, 0, 0, 0xFF };

	if (!gBackgroundImage->loadFromFile("imagenes/init_background.bmp")) {
		Logger::getInstance()->log(Error, "Ocurrio un error al iniciar la imagen de fondo de logueo");
	}

	if (!gResutladoLoginTexture->loadFromRenderedText("Esperando a los demas usuarios...", textColor)) {
		Logger::getInstance()->log(Error, "Ocurrio un error al iniciar la textura Esperando a los demas usuarios");
	}

	rectanguloFullscreen->w = 800;
	rectanguloFullscreen->h = 600;
	return false;
}


void EsperaView::render() {
	//Clear screen
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(gRenderer);

	// Render imagen fondo
	
	gBackgroundImage->render(0, 0, rectanguloFullscreen);

	//Render text textures
	gResutladoLoginTexture->render((SCREEN_WIDTH - gResutladoLoginTexture->getWidth()) / 2, 300);

	//Update screen
	SDL_RenderPresent(gRenderer);
}

void EsperaView::update() {
	
}

bool EsperaView::close() {
	return false;
}

