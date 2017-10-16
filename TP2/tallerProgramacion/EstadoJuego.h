#pragma once
#ifndef ESTADO_JUEGO_H
#define ESTADO_JUEGO_H
#include <string>
#include "Renderer.h"
#include "ManejadorInput.h"

class EstadoJuego {
public:
	virtual void update() = 0;
	virtual void render() = 0;
	virtual bool onEnter(Renderer* renderer) = 0;
	virtual bool onExit() = 0;
	virtual std::string getStateID() const = 0;
protected:
	Renderer* renderer;
};

#endif 