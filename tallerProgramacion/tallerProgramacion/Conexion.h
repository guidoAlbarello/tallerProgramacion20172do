#ifndef CONEXION_H
#define CONEXION_H

#include "Logger.h"
#include "Usuario.h"
#include <thread>
#include "SocketSincronico.h"
#include "ManejadorDeConexionConexion.h"


class Conexion {
public:
	Conexion(SOCKET unSocket);
	void cerrarConexion();
	Usuario* getUsuario() { return usuarioConectado; }
private:
	ManejadorDeConexionConexion* conexionConCliente;
	Usuario* usuarioConectado;
	std::thread t_procesarDatosRecibidos;
	void procesarDatosRecibidos();
	bool conexionActiva;
	SocketSincronico* socket;
};

#endif