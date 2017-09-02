#pragma once
#ifndef  SERVIDOR_H
#define SERVIDOR_H

#include "ManejadorDeConexionServidor.h"
#include "ServerConfig.h"
#include "Usuario.h"
#include "ParserXml.h"
#include "ServerConfig.h"
#include <fstream>
#include <thread>

class Servidor {
public:
	Servidor();
	~Servidor();
	//virtual void iniciarServidor(std::string puertoEscucha, int cantidadMaximaConexiones);
	virtual void iniciarServidor();
	virtual void cerrarServidor();
protected:
	ServerConfig* configuracion;
	ManejadorDeConexionServidor* conexionDelServidor;
	virtual void correrCicloPrincipal();
	bool servidorActivo;
	void leerServerConfig();
	bool existeArchivo(const std::string& nombreDeArchivo);
	std::thread t_procesarDatosRecibidos;
	virtual void procesarDatosRecibidos();
};

#endif