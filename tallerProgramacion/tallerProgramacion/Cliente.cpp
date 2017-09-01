#include "Cliente.h"



Cliente::Cliente(){
	this->conexionDelCliente = new	ManejadorDeConexion();
	this->clienteActivo = true;
}

Cliente::~Cliente() {
	delete this->conexionDelCliente;
}

void Cliente::iniciarCliente(std::string ipServidor, std::string puertoServidor) {
	ParserXml xmlParser;
    clientConfig = xmlParser.openClientConfigFile(DEFAULT_USER_CONFIG_FILE);

	this->conexionDelCliente->iniciarConexionCliente(clientConfig->getIP(), std::to_string(clientConfig->getPuerto()));
	this->correrCicloPrincipal();
}

void Cliente::correrCicloPrincipal() {
}

void Cliente::cerrarCliente() {
	this->conexionDelCliente->cerrarConexion();
	this->clienteActivo = false;
}


//tal vez mas adelante haya que cambiar como inicializamos cuando el cliente se pone activo