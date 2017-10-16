#include "EstadoLogeo.h"

const std::string EstadoLogeo::s_playID = "LOGEO";
void EstadoLogeo::update() {
	loggerView->update();
}

void EstadoLogeo::render() {
	loggerView->render();
}

bool EstadoLogeo::onEnter(Renderer* renderer) {
	//iniciar logeo
	this->loggerView = new LoggerView(this->renderer->getRenderer());
	loggerView->init();
	return true;
}

bool EstadoLogeo::onExit() {
	//cerrar logeo 
	loggerView->close();
	return true;
}