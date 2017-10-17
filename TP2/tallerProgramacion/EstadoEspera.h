#pragma once

#ifndef ESTADO_MENU_H
#define ESTADO_MENU_H
#include"EstadoJuego.h"
#include "ManejadorDeConexionCliente.h"
class EstadoEspera : public EstadoJuego {
public:
	void update(ManejadorDeConexionCliente* conexionCliente);
	void render();
	bool onEnter(Renderer* renderer);
	bool onExit();
	std::string getStateID() const { return s_menuID; }
private:
	static const std::string s_menuID;
};

#endif