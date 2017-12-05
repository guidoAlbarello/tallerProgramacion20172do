#include "EstadoLogeo.h"

const std::string EstadoLogeo::s_playID = "LOGEO";
void EstadoLogeo::update(ManejadorDeConexionCliente* conexionCliente) {
	if (inicializado) {
		loggerView->update();
		Usuario* usuario = loggerView->getUsuario();
		if (usuario != NULL) {
			ManejadorInput::getInstance()->setCerrar(!conexionCliente->login(usuario->getNombre(), usuario->getPassword()));
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
	delete loggerView;
	return true;
}

void EstadoLogeo::setParametro(void * param) {
	if (param != NULL) {
		try {
			string* stringp = (string*)param;
			std::string strNew = stringp->c_str();
			loggerView->setMensajeLogin(strNew);
		} catch (exception e) {
			Logger::getInstance()->log(Error, "Intentaron setear un parametro que no es un String, EstadoLogeo->setParametro");
		}
	}
	//}
	//catch (exception e) {

	//}
}
