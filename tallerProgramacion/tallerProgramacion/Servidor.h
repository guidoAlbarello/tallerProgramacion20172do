#pragma once
#ifndef  SERVIDOR_H
#define SERVIDOR_H

#include "ManejadorDeConexion.h"
#include "ParserXml.h"
#include "ServerConfig.h"
#include "Usuario.h"

const std::string DEFAULT_SERVER_CONFIG_FILE = "server-config.xml";
class Servidor {
public:
	Servidor();
	~Servidor();
	virtual void iniciarServidor();
	virtual void cerrarServidor();
protected:
	ManejadorDeConexion* conexionDelServidor;
	virtual void correrCicloPrincipal();
	ServerConfig* serverConfig;
	bool servidorActivo;
};

#endif