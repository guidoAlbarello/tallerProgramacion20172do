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
#include "Conexion.h"
#include "Logger.h"

class Servidor {
public:
	//virtual void iniciarServidor(std::string puertoEscucha, int cantidadMaximaConexiones);
	static Servidor* getInstance();
	Usuario* buscarUsuario(std::string unUsuario);
	void iniciarServidor();
	void cerrarServidor();

protected:
	ServerConfig* configuracion;
	ManejadorDeConexionServidor* conexionDelServidor;
	bool servidorActivo;
	void leerServerConfig();
	bool existeArchivo(const std::string& nombreDeArchivo);
	Servidor();
	~Servidor();
	static Servidor* instance;
	std::thread t_escucharClientes;
	void escucharClientes();
	void agregarNuevaConexionEntrante(SOCKET unCliente);
	void cerrarTodasLasConexiones();
	void correrCicloPrincipal();
	void cambiarNivelLogeo();
	void mostrarUsuariosConectados();
	void mostrarMenuPrincipal();
	void mostrarMenuModosLogueo();
	void mostrarMenuUsuariosConectados();
	std::vector<Usuario*> listaDeUsuarios;
	std::vector<Conexion*> conexionesActivas;
};

#endif