#pragma once
#ifndef ESCENARIO_H
#define ESCENARIO_H

#include "Ltexture.h"
#include "Renderer.h"

class Escenario {
public:
	Escenario(Renderer* renderer);
	void iniciar();
	~Escenario();
private:
	Ltexture* cielo;
	Ltexture* colinas;
};
#endif