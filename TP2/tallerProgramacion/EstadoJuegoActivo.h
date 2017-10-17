#pragma once
#ifndef ESTADO_JUEGO_ACTIVO_H
#define ESTADO_JUEGO_ACTIVO_H

#include "EstadoJuego.h" 
#include <vector>
#include "Sprite.h"
#include <algorithm>
#include "ManejadorDeConexionCliente.h"
class EstadoJuegoActivo : public EstadoJuego {
public:
	void update(ManejadorDeConexionCliente* conexionCliente);
	void render();
	bool onEnter(Renderer* renderer);
	bool onExit();
	std::string getStateID() const { return s_playID; }
private:
	static const std::string s_playID;
	std::vector<Sprite*> sprites;
	//declarar mapa

	void inicializarMapa();
	void inicializarObjetos();
	void limpiarMapa();
	void limpiarObjetos();
};

#endif