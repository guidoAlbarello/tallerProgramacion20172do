#pragma once

#ifndef ESTADO_ESPERA_H
#define ESTADO_ESPERA_H
#include "EstadoJuego.h"
#include "ManejadorDeConexionCliente.h"
#include "EsperaView.h"

class EstadoEspera : public EstadoJuego {
public:
	void update(ManejadorDeConexionCliente* conexionCliente);
	void render();
	bool onEnter(Renderer* renderer);
	bool onExit();
	std::string getStateID() const { return s_esperaID; }
	std::string s_esperaID = "ESPERA";

private:
	EsperaView* esperaView;
};

#endif