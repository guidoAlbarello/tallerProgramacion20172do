#pragma once
#ifndef ESTADO_JUEGO_ACTIVO_H
#define ESTADO_JUEGO_ACTIVO_H

#include "EstadoJuego.h" 
#include <vector>
#include "Sprite.h"
#include <algorithm>
#include "ManejadorDeConexionCliente.h"
#include "MapaView.h"
#include <map>
#include "Escenario.h"
#include "Camara.h"
#include "ManejadorAudio.h"

class EstadoJuegoActivo : public EstadoJuego {
public:
	void update(ManejadorDeConexionCliente* conexionCliente);
	void render();
	bool onEnter(Renderer* renderer);
	bool onExit();
	std::string getStateID() const { return s_playID; }
	void recieveInput(void* param);
	void cambiarNivel();
	void setParametro(void* param);
	int getCantidadJugadores() { return this->cantJugadores; }
	void setCantidadJugadores(int cantidadJugadores) { this->cantJugadores = cantidadJugadores; }
	void dibujarPantallaTransicion();
	void setNombresJugadores(map<int, string> nombresJugadores) { this->nombresJugadores = nombresJugadores; }
	map<int, string> getNombresJugadores() { return this->nombresJugadores; }
	string obtenerNombreUsuarioPorId(int id, map<int, string> nombresJugadores);
private:
	EstadoModeloJuego* estadoModeloJuego;
	static const std::string s_playID;
	bool estaEnPantallaTransicion = false;
	std::map<int, Sprite*> spritesMap;
	std::vector<Sprite*> spritesVec;
	MapaView* mapaView;
	Escenario* escenario;
	void initPuntajes();
	void inicializarObjetos(EstadoInicialJuego* unEstado);
	void limpiarMapa();
	void limpiarObjetos();
	std::mutex m_estadoModelo;
	Camara* camara;
	int idJugador;
	int cantJugadores;
	map<int, string> nombresJugadores;
	map<int, long> puntajesEtapa1;
	map<int, long> puntajesEtapa2;
	map<int, long> puntajesEtapa3;
	map<int, long> puntajesTotal;
	string idSonidoMotor;
};

#endif