#include "MaquinaEsatdos.h"

void MaquinaEstados::update(ManejadorDeConexionCliente* conexionCliente) {
	m_estado.lock();
	if (!estadosDeJuego.empty()) {
		estadosDeJuego.back()->update(conexionCliente);
	}
	m_estado.unlock();
}
void MaquinaEstados::render() {
	
	if (!estadosDeJuego.empty()) {
		estadosDeJuego.back()->render();
	}
	
}

void MaquinaEstados::pushState(EstadoJuego *pState) {
	m_estado.lock();
	estadosDeJuego.push_back(pState);
	estadosDeJuego.back()->onEnter(this->renderer);
	m_estado.unlock();
}

void MaquinaEstados::popState() {
	m_estado.lock();
	if (!estadosDeJuego.empty()) {
		if (estadosDeJuego.back()->onExit()) {
			delete estadosDeJuego.back();
			estadosDeJuego.pop_back();
		}
	}
	m_estado.unlock();
}

void MaquinaEstados::changeState(EstadoJuego *pState) {
	m_estado.lock();
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
	m_estado.unlock();
}