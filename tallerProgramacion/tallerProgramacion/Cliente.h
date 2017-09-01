#pragma once
#ifndef  CLIENTE_H
#define CLIENTE_H

#include "ManejadorDeConexion.h"
#include "ParserXml.h"
#include "ClientConfig.h"

const std::string DEFAULT_USER_CONFIG_FILE = "client-config.xml";

class Cliente {
public:
	Cliente();
	~Cliente();
	virtual void iniciarCliente(std::string ipServidor, std::string puertoServidor);
	virtual void cerrarCliente();

protected:
	ClientConfig* configuracion;
	ManejadorDeConexion* conexionDelCliente;
	virtual void correrCicloPrincipal();
	ClientConfig* clientConfig;
	bool clienteActivo;
	bool existeArchivo(const std::string& nombreArchivo);
	void leerClientConfig();
};

#endif