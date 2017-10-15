
#pragma once
#ifndef CARRO_H
#define CARRO_H

#include "ObjetoDeJuego.h"
#include "Renderer.h"

//La imagen es de 80  x 41
#define CARRO_X 360
#define CARRO_Y 521
class Carro : public ObjetoDeJuego {
	public:
		Carro(SDL_Renderer *renderer);

		void update(Unidad delta);

	private:
		SDL_Renderer* renderer;
};

	


#endif // !CARRO_H

