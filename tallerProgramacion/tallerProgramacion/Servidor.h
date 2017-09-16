#pragma once
#ifndef  SERVIDOR_H
#define SERVIDOR_H

#include "ManejadorDeConexionServidor.h"
#include "ServerConfig.h"
#include "Usuario.h"
#include "ParserXml.h"
#include "ServerConfig.h"
#include <thread>
#include "Logger.h"
#include "MensajeDeRed.h"

class Conexion;  // Declaracion forward de la clase Conexion


class Servidor {
public:
	static Servidor* getInstance();
	Usuario* buscarUsuario(std::string unUsuario);
	void iniciarServidor();
	void cerrarServidor();
	Usuario* validarLogin(MensajeDeRed* mensaje);
	std::vector<Conexion*> getConexionesActivas();
	std::vector<Usuario*> getUsuarios() { return this->configuracion->getUsuarios(); }
	void recibirMensajeGlobal(string unEmisor, string unMensaje);
	void enviarMensajePrivado(string unDestinatario, string unMensaje);
protected:
	ServerConfig* configuracion;
	BuzonDeMensajes* buzonDeChatGlobal;
	ManejadorDeConexionServidor* conexionDelServidor;
	std::vector<Conexion*> conexionesActivas;
	static Servidor* instance;
	std::thread t_escucharClientes;
	bool servidorActivo;
	Servidor();
	~Servidor();
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
	thread t_enviarChatGlobal;
};

#endif