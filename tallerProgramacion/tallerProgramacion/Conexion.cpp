#include "Conexion.h"

Conexion::Conexion(SOCKET unSocket) {
	this->conexionConCliente = new ManejadorDeConexionConexion(unSocket);
	this->conexionConCliente->iniciarConexion();
	this->conexionActiva = true;
	this->t_procesarDatosRecibidos = std::thread(&Conexion::procesarDatosRecibidos, this);
}

void Conexion::cerrarConexion() {
	this->conexionActiva = false;

	if (t_procesarDatosRecibidos.joinable()) {
		t_procesarDatosRecibidos.join();
	}

	socket->cerrarSocket();
	delete socket;
}

void Conexion::procesarDatosRecibidos() {
	while (conexionActiva) {
		//procesar comandos
	}
}

