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

class EstadoJuegoActivo : public EstadoJuego {
public:
	void update(ManejadorDeConexionCliente* conexionCliente);
	void render();
	bool onEnter(Renderer* renderer);
	bool onExit();
	std::string getStateID() const { return s_playID; }
	void recieveInput(void* param);
	void setParametro(void* param);
private:
	EstadoModeloJuego* estadoModeloJuego;
	static const std::string s_playID;
	std::map<int, Sprite*> spritesMap;
	std::vector<Sprite*> spritesVec;
	MapaView* mapaView;
	Escenario* escenario;
	void inicializarMapa();
	void inicializarObjetos(EstadoInicialJuego* unEstado);
	void limpiarMapa();
	void limpiarObjetos();
	std::mutex m_estadoModelo;
	Camara* camara;
	int idJugador;
};

#endif