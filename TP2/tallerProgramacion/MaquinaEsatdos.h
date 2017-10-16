#pragma once
#ifndef MAQUINA_ESTADOS_H
#define MAQUINA_ESTADOS_H

#include "EstadoJuego.h"
#include <vector>
#include "Renderer.h"

class MaquinaEstados {
public:
	void update();
	void render();
	void pushState(EstadoJuego* pState);
	void changeState(EstadoJuego* pState);
	void popState();
private:
	std::vector<EstadoJuego*> estadosDeJuego;
	Renderer* renderer;
};

#endif