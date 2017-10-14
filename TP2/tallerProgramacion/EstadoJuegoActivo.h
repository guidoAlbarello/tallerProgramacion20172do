#pragma once
#ifndef ESTADO_JUEGO_ACTIVO_H
#define ESTADO_JUEGO_ACTIVO_H

#include "EstadoJuego.h"
class EstadoJuegoActivo : public EstadoJuego {
public:
	void update();
	void render();
	bool onEnter();
	bool onExit();
	std::string getStateID() const { return s_playID; }
private:
	static const std::string s_playID;
};

#endif