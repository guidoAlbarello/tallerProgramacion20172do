#include "Servidor.h"

Servidor::Servidor(){
	this->conexionDelServidor = new	ManejadorDeConexion();
	this->servidorActivo = true;
}

Servidor::~Servidor() {
	delete conexionDelServidor;
}

void Servidor::iniciarServidor() {
	ParserXml* xmlParser = new ParserXml();
	serverConfig = xmlParser->openServerConfigFile(DEFAULT_SERVER_CONFIG_FILE);
	// TODO: definir si el servidor es el que tiene el parser o el main

	this->conexionDelServidor->iniciarConexionServidor(std::to_string(serverConfig->getPuerto()), serverConfig->getMaxClientes());
	this->correrCicloPrincipal();
}

void Servidor::correrCicloPrincipal() {
}

void Servidor::cerrarServidor() {
	this->conexionDelServidor->cerrarConexion();
	this->servidorActivo = false;
}