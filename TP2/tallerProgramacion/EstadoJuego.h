#pragma once
#ifndef ESTADO_JUEGO_H
#define ESTADO_JUEGO_H
#include <string>

class EstadoJuego {
public:
	virtual void update() = 0;
	virtual void render() = 0;
	virtual bool onEnter() = 0;
	virtual bool onExit() = 0;
	virtual std::string getStateID() const = 0;
private:
};

#endif 