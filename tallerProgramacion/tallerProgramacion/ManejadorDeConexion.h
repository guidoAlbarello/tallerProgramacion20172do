#pragma once
#ifndef  MANEJADOR_DE_CONEXION_H
#define MANEJADOR_DE_CONEXION_H


#include "SocketSincronico.h"
#include <thread>
#include <mutex>

enum Comando { LOG = '0', PING = '1', SEND_MESSAGE = '2', RETRIEVE_MESSAGES = '3' };

class ManejadorDeConexion {
public:	
	ManejadorDeConexion();
	ManejadorDeConexion(SOCKET unSocket);
	ManejadorDeConexion(SocketSincronico* unSocket);
	char* getDatosRecibidos() { return bufferDatosRecibidos; } 
	void cerrarConexion();
protected:
	SocketSincronico* socket;
	bool conexionActiva;
	std::thread t_RecibirDatos;
	std::thread t_EnviarDatos;
	virtual void enviarDatos();  //cuando esta sin mandar nada manda cada x segundos un ping q espera respuesta para ver si esta conectado 
	virtual void recibirDatos();

	std::mutex m_bufferDatosAEnviar;
	std::mutex m_bufferDatosRecibidos;

	//enum Comando comandoAEjecutar;
	char* bufferDatosRecibidos;
	char* bufferDatosAEnviar;
	int tamanioDatosAEnviar;
};

#endif

