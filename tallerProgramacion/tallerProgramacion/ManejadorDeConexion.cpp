#include "ManejadorDeConexion.h"

ManejadorDeConexion::ManejadorDeConexion() {
	this->socket = new SocketSincronico();
	conexionActiva = true;
}

ManejadorDeConexion::ManejadorDeConexion(SOCKET unSocket) {
	this->socket = new SocketSincronico(unSocket);
	conexionActiva = true;
}

ManejadorDeConexion::ManejadorDeConexion(SocketSincronico* unSocket) {
	this->socket = unSocket;
	conexionActiva = true;
}

void ManejadorDeConexion::enviarDatos() {
	while (conexionActiva) {
		this->socket->enviarDatos(bufferDatosAEnviar, tamanioDatosAEnviar);
	}
}

void ManejadorDeConexion::recibirDatos() {
	while (conexionActiva) {
		char *datosRecibidos = this->socket->recibirDatos();

		m_bufferDatosRecibidos.lock();
		bufferDatosRecibidos = datosRecibidos;
		m_bufferDatosRecibidos.unlock();
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