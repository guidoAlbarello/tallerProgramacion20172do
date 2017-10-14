#include "EstadoJuegoActivo.h"

const std::string EstadoJuegoActivo::s_playID = "JUEGO_ACTIVO";
void EstadoJuegoActivo::update() {
	// rellenar
}
void EstadoJuegoActivo::render() {
	// rellenar
}
bool EstadoJuegoActivo::onEnter() {
	//iniciarJuego
	return true;
}

bool EstadoJuegoActivo::onExit() {
	//cerrarJuego
	return true;
}