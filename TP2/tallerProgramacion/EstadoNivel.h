#pragma once
#ifndef ESTADO_NIVEL_H
#define ESTADO_NIVEL_H

#include "EstadoJuego.h"

class EstadoNivel : public EstadoJuego {
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