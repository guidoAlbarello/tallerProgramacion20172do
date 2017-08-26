#include "Servidor.h"
#include "Usuario.h"
#include "ParserXml.h"
#include "ServerConfig.h"

const std::string DEFAULT_SERVER_CONFIG_FILE = "server-config.xml";

Servidor::Servidor(){
	this->conexionDelServidor = new	ManejadorDeConexion();
	this->servidorActivo = true;
}

Servidor::~Servidor() {
	delete conexionDelServidor;
}

void Servidor::iniciarServidor(std::string puertoEscucha, int cantidadMaximaConexiones) {
	ParserXml xmlParser;
	ServerConfig serverConfig = *xmlParser.openServerConfigFile(DEFAULT_SERVER_CONFIG_FILE);
	// TODO: definir si el servidor es el que tiene el parser o el main

	this->conexionDelServidor->iniciarConexionServidor(puertoEscucha, cantidadMaximaConexiones);
	this->correrCicloPrincipal();
}

void Servidor::correrCicloPrincipal() {
}

void Servidor::cerrarServidor() {
	this->conexionDelServidor->cerrarConexion();
	this->servidorActivo = false;
}