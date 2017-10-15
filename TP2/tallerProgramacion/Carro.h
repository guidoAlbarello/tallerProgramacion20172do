
#pragma once
#ifndef CARRO_H
#define CARRO_H

#include "ObjetoDeJuego.h"
#include "Renderer.h"

//La imagen es de 80  x 41
#define CARRO_X 360
#define CARRO_Y 521
#define GIRANDO_IZQUIERDA 0
#define DERECHO 1
#define GIRANDO_DERECHA 2

#define PATH_GIRANDO_DERECHA  "imagenes/player_right.png"
#define PATH_DERECHO "imagenes/player_straight.png"
#define PATH_GIRANDO_IZUIERDA "imagenes/player_left.png"

class Carro : public ObjetoDeJuego {
	public:
		
		Carro(SDL_Renderer *renderer);
		void setEstado(int nuevoEstado);
		int getEstado();
		void update(Unidad delta);
	private:
		int estado;
		SDL_Renderer* renderer;
};

	


#endif // !CARRO_H

