#include "MaquinaEsatdos.h"

void MaquinaEstados::update() {
	if (!estadosDeJuego.empty()) {
		estadosDeJuego.back()->update();
	}
}
void MaquinaEstados::render() {
	if (!estadosDeJuego.empty()) {
		estadosDeJuego.back()->render();
	}
}

void MaquinaEstados::pushState(EstadoJuego *pState) {
	estadosDeJuego.push_back(pState);
	estadosDeJuego.back()->onEnter(this->renderer);
}

void MaquinaEstados::popState() {
	if (!estadosDeJuego.empty()) {
		if (estadosDeJuego.back()->onExit()) {
			delete estadosDeJuego.back();
			estadosDeJuego.pop_back();
		}
	}
}

void MaquinaEstados::changeState(EstadoJuego *pState) {
	if (!estadosDeJuego.empty()) {
		if (estadosDeJuego.back()->getStateID() == pState->getStateID()) {
			return; // do nothing
		}
		if (estadosDeJuego.back()->onExit()) {
			delete estadosDeJuego.back();
			estadosDeJuego.pop_back();
		}
	}
	// push back our new state
	estadosDeJuego.push_back(pState);
	// initialise it
	estadosDeJuego.back()->onEnter(this->renderer);
}