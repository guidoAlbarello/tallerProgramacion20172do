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
	void enviarUpdate(EstadoModeloJuego* estado);
	void inicializarClienteJuego(EstadoInicialJuego* estado, Usuario* unUsuario);
	void procesarSolicitudPing();
	bool getConexionActiva() { 	return this->conexionActiva; };
	bool getConexionInicializada() { return this->conexionInicializada; }
	void enviarPantallaTransicion();
	void enviarGameOver();
	void procesarPeticionListaDeUsuarios(std::vector<Jugador*> jugadores);
	bool getEnviarPing() { return enviarPing; }
private:
	bool clienteCerroConexion = false;
	ManejadorDeConexionConexion* conexionConCliente;
	void procesarInput(bool* entrada);
	void procesarSend_Message(MensajeDeRed* unMensajeDeRed);
	void procesarRetrieve_Messages(MensajeDeRed* unMensajeDeRed);
	Usuario* usuarioConectado;
	std::thread t_procesarDatosRecibidos;
	void procesarDatosRecibidos();
	bool conexionInicializada = false;
	bool conexionActiva;
	bool conexionViva;
	bool enviarPing = false;
	bool cerrando = false;
	Servidor* servidor;

};

#endif