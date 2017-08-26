#include "Servidor.h"

Servidor::Servidor(){
	this->conexionDelServidor = new	ManejadorDeConexion();
	this->servidorActivo = true;
}

Servidor::~Servidor() {
	delete conexionDelServidor;
}

void Servidor::iniciarServidor(std::string puertoEscucha, int cantidadMaximaConexiones) {
	this->conexionDelServidor->iniciarConexionServidor(puertoEscucha, cantidadMaximaConexiones);
	this->correrCicloPrincipal();
}

void Servidor::correrCicloPrincipal() {
}

void Servidor::cerrarServidor() {
	this->conexionDelServidor->cerrarConexion();
	this->servidorActivo = false;
}