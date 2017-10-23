#include "EsperaView.h"

EsperaView::EsperaView(SDL_Renderer* renderer) {
	this->gRenderer = renderer;
	this->gResutladoLoginTexture = new Ltexture(gRenderer);
}

EsperaView::~EsperaView() {
	if (gResutladoLoginTexture != NULL) {
		gResutladoLoginTexture->free();
	}

	TTF_CloseFont(gFont);
	gFont = NULL;
}

bool EsperaView::init() {
	textColor = { 0, 0, 0, 0xFF };
	gResutladoLoginTexture->loadFromRenderedText("Esperando a los demas usuarios...", textColor);
	return false;
}


void EsperaView::render() {
	//Clear screen
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(gRenderer);

	//Render text textures
	gResutladoLoginTexture->render((SCREEN_WIDTH - gResutladoLoginTexture->getWidth()) / 2, 0);

	//Update screen
	SDL_RenderPresent(gRenderer);
}

void EsperaView::update() {
	
}

bool EsperaView::close() {
	return false;
}

