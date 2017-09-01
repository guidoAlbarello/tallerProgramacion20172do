#pragma once
#ifndef  CLIENTE_H
#define CLIENTE_H

#include "ManejadorDeConexionCliente.h"
#include "ParserXml.h"
#include "ClientConfig.h"
#include <fstream>

const std::string DEFAULT_CLIENT_CONFIG_FILE = "client-config.xml";

const std::string DEFAULT_USER_CONFIG_FILE = "client-config.xml";
class Cliente {
public:
	Cliente();
	~Cliente();
	virtual void iniciarCliente(std::string ipServidor, std::string puertoServidor);
	virtual void cerrarCliente();

protected:
	ManejadorDeConexionCliente* conexionDelCliente;
	virtual void correrCicloPrincipal();
	ClientConfig* configuracion;
	ClientConfig* clientConfig;
	bool clienteActivo;
	bool existeArchivo(const std::string& nombreArchivo);
	void leerClientConfig();
};

#endif