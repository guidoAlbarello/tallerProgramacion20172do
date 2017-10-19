#ifndef CONEXION_H
#define CONEXION_H

#include "Logger.h"
#include "Usuario.h"
#include <thread>
#include "SocketSincronico.h"
#include "ManejadorDeConexionConexion.h"
#include "ParserDeMensajes.h"
#include "Servidor.h"
#include <chrono>

class Servidor;  // Declaracion forward de la clase Servidor


class Conexion {
public:
	Conexion(SOCKET unSocket, Servidor* servidor);
	~Conexion();
	void cerrarConexion();
	Usuario* getUsuario() { return usuarioConectado; }
	ManejadorDeConexionConexion* getConexionConCliente() { return this->conexionConCliente; };
	void enviarChatGlobal(bool tipoDeChat, string unEmisor, string unMensaje);
	void enviarUpdate(EstadoModeloJuego estado);
	void procesarSolicitudPing();
	bool getConexionActiva() { 	return this->conexionActiva; };
private:
	ManejadorDeConexionConexion* conexionConCliente;
	void procesarInput(MensajeDeRed* unMensajeDeRed);
	void procesarSend_Message(MensajeDeRed* unMensajeDeRed);
	void procesarRetrieve_Messages(MensajeDeRed* unMensajeDeRed);
	void procesarPeticionListaDeUsuarios();
	Usuario* usuarioConectado;
	std::thread t_procesarDatosRecibidos;
	void procesarDatosRecibidos();
	bool conexionActiva;
	bool conexionViva;
	Servidor* servidor;

};

#endif