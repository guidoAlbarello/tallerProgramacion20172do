#include "ManejadorDeConexion.h"

ManejadorDeConexion::ManejadorDeConexion() {
	this->socket = new SocketSincronico();
	bufferDatosAEnviar = NULL;
	bufferDatosRecibidos = NULL;
	conexionActiva = true;
}

ManejadorDeConexion::ManejadorDeConexion(SOCKET unSocket) {
	this->socket = new SocketSincronico(unSocket);
	bufferDatosAEnviar = NULL;
	bufferDatosRecibidos = NULL;
	conexionActiva = true;
}

ManejadorDeConexion::ManejadorDeConexion(SocketSincronico* unSocket) {
	this->socket = unSocket;
	bufferDatosAEnviar = NULL;
	bufferDatosRecibidos = NULL;
	conexionActiva = true;
}

void  ManejadorDeConexion::notificarDatosAEnviar() {
	condicion_enviar.notify_all();
}

void  ManejadorDeConexion::notificarDatoARecibir() {
	condicion_recibir.notify_all();
}

char* ManejadorDeConexion::getMensaje() {
	char* primerMensaje = NULL;
	m_bufferDatosRecibidos.lock();
	if (this->mensajesEntrantes.size() > 0) {
		primerMensaje = this->mensajesEntrantes.at(0);
		this->mensajesEntrantes.erase(this->mensajesEntrantes.begin());
	}
	m_bufferDatosRecibidos.unlock();
	return primerMensaje;
}

void ManejadorDeConexion::enviarDatos() {
	/*
	//Por ahora no se usa.. quizas en la 2da entrega
	while (conexionActiva) {
	condicion_enviar.wait(m_bufferDatosAEnviar);
	//espera hasta que alguien haga condicion_enviar.notifyAll();
	if (bufferDatosAEnviar != NULL) {
	if (this->socket->enviarDatos(bufferDatosAEnviar, tamanioDatosAEnviar)) {
	bufferDatosAEnviar = NULL;
	}
	}
	}
	*/
}

void ManejadorDeConexion::recibirDatos() {

	while (conexionActiva) {

		char *datosRecibidos = this->socket->recibirDatos();
		if (datosRecibidos != NULL) {
			m_bufferDatosRecibidos.lock();
			mensajesEntrantes.push_back(datosRecibidos);
			m_bufferDatosRecibidos.unlock();
		}
	}
}

void ManejadorDeConexion::cerrarConexion() {
	this->conexionActiva = false;

	if (t_EnviarDatos.joinable()) {
		t_EnviarDatos.join();
	}

	if (t_RecibirDatos.joinable()) {
		t_RecibirDatos.join();
	}

	for (int i = 0; i < this->mensajesEntrantes.size(); i++) {
		delete this->mensajesEntrantes.at(i);
	}

	if (mensajesEntrantes.size() > 0)
		this->mensajesEntrantes.erase(this->mensajesEntrantes.begin(), this->mensajesEntrantes.end());

	if (this->socket != NULL) {
		socket->cerrarSocket();
		this->borrarEntorno();
		delete socket;
	}
}

void ManejadorDeConexion::borrarEntorno() {
}
