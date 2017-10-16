#include "EstadoJuegoActivo.h"

const std::string EstadoJuegoActivo::s_playID = "JUEGO_ACTIVO";
void EstadoJuegoActivo::update() {
	// updatear modelo
}
void EstadoJuegoActivo::render() {
	sort(sprites.begin(),sprites.end(), [](Sprite* a, Sprite* b)->bool {			
		return a->getZIndex() > b->getZIndex();
	});
	
	for (int i = 0; i < sprites.size(); i++) {
		sprites[i]->render(this->renderer);
	}
}

bool EstadoJuegoActivo::onEnter(Renderer* renderer) {
	inicializarMapa();
	inicializarObjetos();
	this->renderer = renderer;
	return true;
}

bool EstadoJuegoActivo::onExit() {
	limpiarMapa();
	limpiarObjetos();
	return true;
}

void EstadoJuegoActivo::inicializarMapa() {//inicializar la estructura de datos de mapa
}

void EstadoJuegoActivo::inicializarObjetos() {//inicializar la estrucutra de datos de objetos

}

void EstadoJuegoActivo::limpiarMapa() {//liberar memoria mapa
}

void EstadoJuegoActivo::limpiarObjetos() {//liberar memoria objetos
}
