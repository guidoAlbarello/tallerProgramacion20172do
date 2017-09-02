#pragma once
#ifndef  CLIENTE_H
#define CLIENTE_H

#include "ManejadorDeConexionCliente.h"
#include "ParserXml.h"
#include "ClientConfig.h"
#include <fstream>
#include <thread>
#include "Logger.h"

const std::string DEFAULT_CLIENT_CONFIG_FILE = "client-config.xml";

const std::string DEFAULT_USER_CONFIG_FILE = "client-config.xml";
class Cliente {
public:
	static Cliente* getInstance();
	void iniciarCliente();
	void cerrarCliente();

protected:
	ManejadorDeConexionCliente* conexionDelCliente;
	virtual void correrCicloPrincipal();
	ClientConfig* configuracion;
	ClientConfig* clientConfig;
	bool clienteActivo;
	bool existeArchivo(const std::string& nombreArchivo);
	void leerClientConfig();
	std::thread t_procesarDatosRecibidos;
	Cliente();
	~Cliente();
	static Cliente* instance;
	void mostrarMenu();
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