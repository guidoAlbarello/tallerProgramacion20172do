#pragma once

#ifndef SPRITE_H
#define SPRITE_H
#include "ObjetoDeJuego.h"
#include "Renderer.h"

class Renderer;

class Sprite : public ObjetoDeJuego {
public:
	void render(Renderer* renderer);
private:

};

#endif