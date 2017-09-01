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
		char* datosAEnviar = procesarDatosAEnviar();
		int tamanioDatosAEnviar = 0;
		this->socket->enviarDatos(datosAEnviar, tamanioDatosAEnviar);
	}
}

void ManejadorDeConexion::recibirDatos() {
	while (conexionActiva) {
		char* datosRecibidos = this->socket->recibirDatos();
		procesarDatosRecibidos(datosRecibidos);
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