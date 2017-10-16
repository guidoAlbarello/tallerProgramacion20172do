#pragma once
#ifndef ESTADO_LOGEO_H
#define ESTADO_LOGEO_H
#include "EstadoJuego.h"
#include "LoggerView.h"
class EstadoLogeo : public EstadoJuego {
public:
	void update();
	void render();
	bool onEnter(Renderer* renderer);
	bool onExit();
	std::string getStateID() const { return s_playID; }
private:
	static const std::string s_playID;
	LoggerView* loggerView;
};

#endif