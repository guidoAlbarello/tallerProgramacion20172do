#include "EstadoEspera.h"

void EstadoEspera::update(ManejadorDeConexionCliente* conexionCliente) {
	this->esperaView->update();
}
void EstadoEspera::render() {
	esperaView->render();
}
bool EstadoEspera::onEnter(Renderer* renderer) {
	this->renderer = renderer;
	this->esperaView = new EsperaView(this->renderer->getRendererJuego());
	esperaView->init();
	return true;
}
bool EstadoEspera::onExit() {
	esperaView->close();
	return true;
}