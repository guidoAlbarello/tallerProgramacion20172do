#include "EstadoLogeo.h"

const std::string EstadoLogeo::s_playID = "LOGEO";
void EstadoLogeo::update(ManejadorDeConexionCliente* conexionCliente) {
	if (inicializado) {
		loggerView->update();
		Usuario* usuario = loggerView->getUsuario();
		if (usuario != NULL) {
			conexionCliente->login(usuario->getNombre(), usuario->getPassword());
			inicializado = false;
		}
	}
}

void EstadoLogeo::render() {
	if(inicializado)
		loggerView->render();
}

bool EstadoLogeo::onEnter(Renderer* renderer) {
	//iniciar logeo
	this->renderer = renderer;
	this->loggerView = new LoggerView(this->renderer->getRendererJuego());
	loggerView->init();
	inicializado = true;
	return true;
}

bool EstadoLogeo::onExit() {
	//cerrar logeo 
	loggerView->close();
	return true;
}