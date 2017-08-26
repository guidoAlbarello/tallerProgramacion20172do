#pragma once
#ifndef  SERVIDOR_H
#define SERVIDOR_H

#include "ManejadorDeConexion.h"

class Servidor {
public:
	Servidor();
	~Servidor();
	virtual void iniciarServidor(std::string puertoEscucha, int cantidadMaximaConexiones);
	virtual void cerrarServidor();
protected:
	ManejadorDeConexion* conexionDelServidor;
	virtual void correrCicloPrincipal();
	bool servidorActivo;
};

#endif