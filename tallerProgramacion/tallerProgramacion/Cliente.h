#pragma once
#ifndef  CLIENTE_H
#define CLIENTE_H

#include "ManejadorDeConexion.h"

class Cliente {
public:
	Cliente();
	~Cliente();
	virtual void iniciarCliente(std::string ipServidor, std::string puertoServidor);
	virtual void cerrarCliente();

protected:
	ManejadorDeConexion* conexionDelCliente;
	virtual void correrCicloPrincipal();
	bool clienteActivo;
};

#endif