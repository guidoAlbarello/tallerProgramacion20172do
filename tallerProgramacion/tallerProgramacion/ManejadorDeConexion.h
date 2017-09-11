#pragma once
#ifndef  MANEJADOR_DE_CONEXION_H
#define MANEJADOR_DE_CONEXION_H


#include "SocketSincronico.h"
#include <thread>
#include <mutex>


class ManejadorDeConexion {
public:	
	ManejadorDeConexion();
	ManejadorDeConexion(SOCKET unSocket);
	ManejadorDeConexion(SocketSincronico* unSocket);
	char* getDatosRecibidos() { return bufferDatosRecibidos; } 
	void setDatosRecibidos(char* datos) { bufferDatosRecibidos = datos; };
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

