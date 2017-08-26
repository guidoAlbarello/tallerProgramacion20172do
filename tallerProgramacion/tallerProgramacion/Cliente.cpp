#include "Cliente.h"

Cliente::Cliente(){
	this->conexionDelCliente = new	ManejadorDeConexion();
	this->clienteActivo = true;
}

Cliente::~Cliente() {
	delete this->conexionDelCliente;
}

void Cliente::iniciarCliente(std::string ipServidor, std::string puertoServidor) {
	this->conexionDelCliente->iniciarConexionCliente(ipServidor, puertoServidor);
	this->correrCicloPrincipal();
}

void Cliente::correrCicloPrincipal() {
}

void Cliente::cerrarCliente() {
	this->conexionDelCliente->cerrarConexion();
	this->clienteActivo = false;
}


//tal vez mas adelante haya que cambiar como inicializamos cuando el cliente se pone activo