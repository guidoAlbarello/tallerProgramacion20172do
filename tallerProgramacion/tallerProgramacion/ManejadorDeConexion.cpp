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

void ManejadorDeConexion::enviarDatos() {
	while (conexionActiva) {
		if (bufferDatosAEnviar != NULL) {
			if (this->socket->enviarDatos(bufferDatosAEnviar, tamanioDatosAEnviar)) {
				bufferDatosAEnviar = NULL;
			}
		}
	}
}

void ManejadorDeConexion::recibirDatos() {
	while (conexionActiva) {
		char *datosRecibidos = this->socket->recibirDatos();
		if (datosRecibidos != NULL) {
			m_bufferDatosRecibidos.lock();
			bufferDatosRecibidos = datosRecibidos;
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

	socket->cerrarSocket();
	delete socket;
}