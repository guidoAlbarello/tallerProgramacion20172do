#pragma once
#ifndef  CLIENTE_H
#define CLIENTE_H

#include "ManejadorDeConexionCliente.h"
#include "BuzonDeMensajes.h"
#include "ClientConfig.h"
#include <thread>
#include <iostream>
#include <limits>
#include "Logger.h"
#include "MensajeDeRed.h"
#include "Renderer.h"
#include "LoggerView.h"
#include "MaquinaEstados.h"
#include "Ltexture.h"
#include "EstadoJuegoActivo.h"
#include "EstadoLogeo.h"
#include "EstadoEspera.h"
#include "ManejadorInput.h"
const std::string CLIENT_LOG_FILENAME_FORMAT = "LogCl%Y%m%d%H%M%S.txt";

class Cliente {
public:
	static Cliente* getInstance();
	void iniciarCliente();
	void cerrarCliente();

protected:
	bool clienteDecideCerrar = false;
	bool enPantallaDeTransicion = false;
	MaquinaEstados* maquinaDeEstados;
	void mostrarMensajesPrivados(MensajeDeRed* unMensajeDeRed);
	void procesarMensajesGlobales(MensajeDeRed* unMensajeDeRed);
	void procesarMensajesPrivados(MensajeDeRed* unMensajeDeRed);
	void procesarResultadoSendMessage(MensajeDeRed* mensajeDeRed);
	void procesarResultadoLogin(MensajeDeRed* mensajeDeRed, char* datosRecibidos);
	void procesarEstadoModelo(EstadoModeloJuego* estadoModeloJuego);
	void iniciarJuego(EstadoInicialJuego* unEstadoInicial);
	int idJugador = -1;
	ManejadorDeConexionCliente* conexionDelCliente;
	BuzonDeMensajes* buzonDeMensajesGlobales;
	virtual void correrCicloPrincipal();
	ClientConfig* configuracion;
	bool clienteActivo;
	bool estaLogueado;
	bool conexionViva; // Manejada por los pings
	bool juegoIniciado;
	void leerClientConfig();
	std::thread t_procesarDatosRecibidos;
	std::thread t_procesarPing;
	std::mutex m_procesarPing;
	std::mutex m_print;
	std::mutex m_init_juego;
	std::thread t_render;
	void render();
	Cliente();
	~Cliente();
	static Cliente* instance;
	void mostrarMenuPrincipal();
	void mostrarMenuLogin();
	void mostrarMenuBuzon();
	void mostrarUsuariosConectados(MensajeDeRed * unMensajeDeRed);
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
	bool existeArchivo(const std::string& nombre);
	void enviarPingAServidor();
	void leerTestXML(std::string stressFileName, int stressTestTime);
	void procesarNombresUsuario(MensajeDeRed* mensajeDeRed);
	bool dibujarGameOver = false;
	Renderer* renderer;
	std::map<int, string> nombreJugadores;
};

#endif