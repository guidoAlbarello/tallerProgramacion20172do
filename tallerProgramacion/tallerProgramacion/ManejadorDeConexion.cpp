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

void ManejadorDeConexion::iniciarConexionCliente(std::string ipServidor, std::string puertoServidor) {
	this->socket->crearSocketCliente(ipServidor, puertoServidor);

	this->t_EnviarDatos = std::thread(&ManejadorDeConexion::enviarDatos, this);
	this->t_RecibirDatos = std::thread(&ManejadorDeConexion::recibirDatos, this);
}

void ManejadorDeConexion::iniciarConexionServidor(std::string puertoEscucha, int cantidadConexionesMaxima) {
	this->socket->crearSocketServidor(puertoEscucha, cantidadConexionesMaxima);

	this->t_EnviarDatos = std::thread(&ManejadorDeConexion::enviarDatos, this);
	this->t_RecibirDatos = std::thread(&ManejadorDeConexion::recibirDatos, this);
}

void ManejadorDeConexion::enviarDatos() {
	while (conexionActiva) {
		const char* datosAEnviar = NULL;
		int tamanioDatosAEnviar = 0;
		this->socket->enviarDatos(datosAEnviar, tamanioDatosAEnviar);
	}
}

void ManejadorDeConexion::recibirDatos() {
	while (conexionActiva) {
		const char* datosRecibidos = this->socket->recibirDatos();
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