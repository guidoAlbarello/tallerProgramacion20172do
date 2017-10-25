#pragma once
#ifndef MAQUINA_ESTADOS_H
#define MAQUINA_ESTADOS_H

#include "EstadoJuego.h"
#include <vector>
#include "Renderer.h"
#include "ManejadorDeConexionCliente.h"
#include <mutex>
class MaquinaEstados {
public:
	void update(ManejadorDeConexionCliente* conexionCliente);
	void render();
	void pushState(EstadoJuego* pState);
	void changeState(EstadoJuego* pState, void* param = NULL);
	void popState();
	void setRenderer(Renderer* renderer) { this->renderer = renderer; }
	void recieveInput(void* param);
private:
	std::vector<EstadoJuego*> estadosDeJuego;
	Renderer* renderer;
	std::mutex m_estado;
	bool terminoIniciar;
};

#endif