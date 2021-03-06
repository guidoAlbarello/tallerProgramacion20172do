#pragma once
#ifndef SERVIDOR_H
#define SERVIDOR_H

#include "ManejadorDeConexionServidor.h"
#include "ServerConfig.h"
#include "Usuario.h"
#include "ServerConfig.h"
#include <thread>
#include "Logger.h"
#include "MensajeDeRed.h"

const std::string SERVER_LOG_FILENAME_FORMAT = "LogSrv%Y%m%d%H%M%S.txt";

class Conexion;  // Declaracion forward de la clase Conexion


class Servidor {
public:
	static Servidor* getInstance();
	Usuario* buscarUsuario(std::string unUsuario);
	void iniciarServidor();
	void cerrarServidor();
	Usuario* validarLogin(MensajeDeRed* mensaje, string &mensajeResultado);
	std::vector<Conexion*> getConexionesActivas();
	std::vector<Usuario*> getUsuarios() { return this->configuracion->getUsuarios(); }
	void recibirMensajeGlobal(string unEmisor, string unMensaje);
	void enviarMensajePrivado(string unDestinatario, string unMensaje);
	std::vector<Conexion*> conexionesActivas;
	bool estaElUsuarioConectado(Usuario* unUsuario);
protected:
	Servidor();
	~Servidor();
	ServerConfig* configuracion;
	BuzonDeMensajes* buzonDeChatGlobal;
	ManejadorDeConexionServidor* conexionDelServidor;
	static Servidor* instance;
	std::thread t_escucharClientes;
	std::thread t_enviarChatGlobal;
	bool servidorActivo;
	void leerServerConfig();
	void escucharClientes();
	void agregarNuevaConexionEntrante(SOCKET unCliente);
	void cerrarTodasLasConexiones();
	void correrCicloPrincipal();
	void cambiarNivelLogeo();
	void mostrarUsuariosConectados();
	void mostrarMenuPrincipal();
	void mostrarMenuModosLogueo();
	void mostrarMenuUsuariosConectados();
	Usuario* usuarioValido(std::string usuario, std::string contrasenia);
	void enviarChatGlobal();
	void verificarConexiones();
};

#endif