#pragma once
#ifndef  CLIENTE_H
#define CLIENTE_H

#include "ManejadorDeConexionCliente.h"
#include "ParserXml.h"
#include "ClientConfig.h"
#include <thread>


class Cliente {
public:
	static Cliente* getInstance();
	void iniciarCliente();
	void cerrarCliente();

protected:
	ManejadorDeConexionCliente* conexionDelCliente;
	virtual void correrCicloPrincipal();
	ClientConfig* configuracion;
	bool clienteActivo;
	void leerClientConfig();
	std::thread t_procesarDatosRecibidos;
	Cliente();
	~Cliente();
	static Cliente* instance;
	void mostrarMenuPrincipal();
	void mostrarMenuLogin();
	void mostrarMenuBuzon();
	void mostrarMenuMensajeChat();
	void mostrarMenuMensajePrivado();
	void conectarseAlServidor();
	void desconectarseDelServidor();
	void hacerTestDeEstres();
	void revisarBuzon();
	void logearseAlServidor();
	void enviarMensajeAlChat();
	void enviarMensajePrivado();
	void procesarDatosRecibidos();
};

#endif