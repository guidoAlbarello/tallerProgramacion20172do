#include "EstadoEspera.h"

const std::string EstadoEspera::s_menuID = "MENU";
void EstadoEspera::update(ManejadorDeConexionCliente* conexionCliente) {
	// rellenar
}
void EstadoEspera::render() {
	// rellenar
}
bool EstadoEspera::onEnter(Renderer* renderer) {
	//iniciarMenu
	return true;
}
bool EstadoEspera::onExit() {
	//cerrarmenu
	return true;
}