#pragma once
#ifndef ESTADO_JUEGO_H
#define ESTADO_JUEGO_H
#include <string>
#include "Renderer.h"
#include "ManejadorInput.h"
#include "ManejadorDeConexionCliente.h"
class EstadoJuego {
public:
	virtual void update(ManejadorDeConexionCliente* conexionCliente) = 0;
	virtual void render() = 0;
	virtual bool onEnter(Renderer* renderer) = 0;
	virtual bool onExit() = 0;
	virtual std::string getStateID() const = 0;
	void setRenderer(Renderer* renderer) { this->renderer = renderer; }
	virtual void recieveInput(void* param) {}
	virtual void setParametro(void* param) {}
protected:
	Renderer* renderer;
	bool inicializado = false;
};

#endif 