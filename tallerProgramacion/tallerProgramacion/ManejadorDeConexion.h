#pragma once
#ifndef  MANEJADOR_DE_CONEXION_H
#define MANEJADOR_DE_CONEXION_H


#include "SocketSincronico.h"
#include <thread>

class ManejadorDeConexion {
public:	
	ManejadorDeConexion();
	ManejadorDeConexion(SOCKET unSocket);
	ManejadorDeConexion(SocketSincronico* unSocket);
	void iniciarConexionServidor(std::string puertoEscucha, int cantidadConexionesMaxima);
	void iniciarConexionCliente(std::string ipServidor, std::string puertoServidor);
	void cerrarConexion();
private:
	SocketSincronico* socket;
	bool conexionActiva;
	std::thread t_RecibirDatos;
	std::thread t_EnviarDatos;
	void enviarDatos();
	void recibirDatos();
};

#endif

