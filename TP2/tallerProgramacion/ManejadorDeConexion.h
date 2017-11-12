
#pragma once
#ifndef  MANEJADOR_DE_CONEXION_H
#define MANEJADOR_DE_CONEXION_H

#define USAR_COLA_MENSAJES true
#define LIMITE_COLA_MENSAJES 2
#include "SocketSincronico.h"
#include <thread>
#include <mutex>
#include <condition_variable> // std::condition_variable
#include <vector>

class ManejadorDeConexion {
public:
	ManejadorDeConexion();
	ManejadorDeConexion(SOCKET unSocket);
	ManejadorDeConexion(SocketSincronico* unSocket);
	char* getDatosRecibidos() { return bufferDatosRecibidos; }
	void setDatosRecibidos(char* datos) { bufferDatosRecibidos = datos; };
	virtual void cerrarConexion();
	virtual void borrarEntorno();
	SocketSincronico getSocket() { return *this->socket; };
	bool getConexionActiva() { return this->conexionActiva; };
	void setConexionActiva(bool conexionActiva) { this->conexionActiva = conexionActiva; }
	void notificarDatosAEnviar();
	void notificarDatoARecibir();
	char* getMensaje();
protected:
	SocketSincronico* socket;
	bool conexionActiva;
	std::thread t_RecibirDatos;
	std::thread t_EnviarDatos;
	virtual void enviarDatos();  //cuando esta sin mandar nada manda cada x segundos un ping q espera respuesta para ver si esta conectado 
	virtual void recibirDatos();

	std::mutex m_bufferDatosAEnviar;
	std::mutex m_bufferDatosRecibidos;
	std::condition_variable condicion_enviar;
	std::condition_variable condicion_recibir;
	std::vector<char*> mensajesEntrantes;

	char* mensajeRecibido;
	char* bufferDatosRecibidos;
	char* bufferDatosAEnviar;
	int tamanioDatosAEnviar;
};

#endif
