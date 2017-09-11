#ifndef CONEXION_H
#define CONEXION_H

#include "Logger.h"
#include "Usuario.h"
#include <thread>
#include "SocketSincronico.h"
#include "ManejadorDeConexionConexion.h"

class Servidor;  // Declaracion forward de la clase Servidor


class Conexion {
public:
	Conexion(SOCKET unSocket, Servidor* servidor);
	void cerrarConexion();
	Usuario* getUsuario() { return usuarioConectado; }
private:
	ManejadorDeConexionConexion* conexionConCliente;
	Usuario* usuarioConectado;
	std::thread t_procesarDatosRecibidos;
	void procesarDatosRecibidos();
	bool conexionActiva;
	SocketSincronico* socket;
	Servidor* servidor;
};

#endif