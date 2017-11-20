#pragma once
#ifndef ESTADO_PANTALLA_NIVEL_H
#define ESTADO_PANTALLA_NIVEL_H

#include "EstadoJuego.h"

class EstadoPantallaNivel : public EstadoJuego {
public:
	void update();
	void render();
	bool onEnter();
	bool onExit();
	std::string getStateID() const { return s_playID; }
	void setParametro(void* param);
private:
	static const std::string s_playID;
};

#endif