#pragma once
#ifndef  SERVIDOR_H
#define SERVIDOR_H

#include "ManejadorDeConexion.h"
#include "ServerConfig.h"

class Servidor {
public:
	Servidor();
	~Servidor();
	//virtual void iniciarServidor(std::string puertoEscucha, int cantidadMaximaConexiones);
	virtual void iniciarServidor();
	virtual void cerrarServidor();
protected:
	ServerConfig* configuracion;
	ManejadorDeConexion* conexionDelServidor;
	virtual void correrCicloPrincipal();
	bool servidorActivo;
	void leerServerConfig();
	bool existeArchivo(const std::string& nombreDeArchivo);
};

#endif