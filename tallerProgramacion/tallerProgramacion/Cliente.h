#pragma once
#ifndef  CLIENTE_H
#define CLIENTE_H

#include "ManejadorDeConexionCliente.h"
#include "ParserXml.h"
#include "ClientConfig.h"
#include <thread>
#include <iostream>
#include <limits>
#include "Logger.h"
#include "MensajeDeRed.h"

class Cliente {
public:
	static Cliente* getInstance();
	void iniciarCliente();
	void cerrarCliente();

protected:
	void mostrarMensajesPrivados(MensajeDeRed* unMensajeDeRed);
	void procesarMensajesGlobales(MensajeDeRed* unMensajeDeRed);
	void procesarMensajesPrivados(MensajeDeRed* unMensajeDeRed);
	ManejadorDeConexionCliente* conexionDelCliente;
	BuzonDeMensajes* buzonDeMensajesGlobales;
	virtual void correrCicloPrincipal();
	ClientConfig* configuracion;
	bool clienteActivo;
	bool estaLogueado;
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
	void mostrarMensajesGlobales();
	bool enviandoMensaje;

};

#endif