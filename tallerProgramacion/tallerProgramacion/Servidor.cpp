#include "Servidor.h"
#include "Usuario.h"
#include "ParserXml.h"
#include "ServerConfig.h"
#include <fstream>

const std::string DEFAULT_SERVER_CONFIG_FILE = "server-config.xml";

Servidor::Servidor() {
	this->conexionDelServidor = new	ManejadorDeConexion();
	this->servidorActivo = true;
}

Servidor::~Servidor() {
	delete conexionDelServidor;
}

void Servidor::iniciarServidor() {
	// TODO: definir si el servidor es el que tiene el parser o el main
	this->leerServerConfig();

	this->conexionDelServidor->iniciarConexionServidor(this->configuracion->getPuerto(), this->configuracion->getMaxClientes());
	this->correrCicloPrincipal();
}

void Servidor::correrCicloPrincipal() {
}

void Servidor::cerrarServidor() {
	this->conexionDelServidor->cerrarConexion();
	this->servidorActivo = false;
}

void Servidor::leerServerConfig() {
	if (this->existeArchivo(DEFAULT_SERVER_CONFIG_FILE)) {
		ParserXml* xmlParser = new ParserXml();
		this->configuracion = xmlParser->readServerConfigFile(DEFAULT_SERVER_CONFIG_FILE);
	}
	else {
		this->configuracion = new ServerConfig();
		this->configuracion->crearArchivoConfiguracion(DEFAULT_SERVER_CONFIG_FILE);
	}
}

bool Servidor::existeArchivo(const std::string& nombreDeArchivo) {
	std::ifstream archivo(nombreDeArchivo.c_str());
	return (bool)archivo;
}