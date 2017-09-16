#ifndef CONEXION_H
#define CONEXION_H

#include "Logger.h"
#include "Usuario.h"
#include <thread>
#include "SocketSincronico.h"
#include "ManejadorDeConexionConexion.h"
#include "ParserDeMensajes.h"
#include "Servidor.h"
class Servidor;  // Declaracion forward de la clase Servidor


class Conexion {
public:
	Conexion(SOCKET unSocket, Servidor* servidor);
	void cerrarConexion();
	Usuario* getUsuario() { return usuarioConectado; }
	ManejadorDeConexionConexion* getConexionConCliente() { return this->conexionConCliente; };
	void enviarChatGlobal(bool tipoDeChat, string unEmisor, string unMensaje);
private:
	ManejadorDeConexionConexion* conexionConCliente;
	void procesarPing();
	void procesarSend_Message(MensajeDeRed* unMensajeDeRed);
	void procesarRetrieve_Messages(MensajeDeRed* unMensajeDeRed);
	Usuario* usuarioConectado;
	std::thread t_procesarDatosRecibidos;
	void procesarDatosRecibidos();
	bool conexionActiva;
	Servidor* servidor;
};

#endif