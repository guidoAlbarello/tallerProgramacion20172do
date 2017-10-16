#pragma once
#ifndef ESCENARIO_H
#define ESCENARIO_H

#include "Ltexture.h"
#include "Renderer.h"
#include "ObjetoDeJuego.h"

class Escenario : public ObjetoDeJuego {
public:
	Escenario(Renderer* renderer);
	void iniciar();
	~Escenario();
	void update(Unidad delta);
	void leerEntrada(std::vector<SDL_Keycode> entradas);
private:
	Ltexture* cielo;
	Ltexture* colinas;
};
#endif