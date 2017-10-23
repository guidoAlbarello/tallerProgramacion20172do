#include "EstadoEspera.h"

const std::string EstadoEspera::s_menuID = "MENU";
void EstadoEspera::update(ManejadorDeConexionCliente* conexionCliente) {
	// rellenar
}
void EstadoEspera::render() {
	// rellenar
	int i = 0; // borrame!
}
bool EstadoEspera::onEnter(Renderer* renderer) {
	//iniciarMenu
	this->renderer = renderer;

	return true;
}
bool EstadoEspera::onExit() {
	//cerrarmenu
	return true;
}