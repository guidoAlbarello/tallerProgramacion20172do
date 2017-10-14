#pragma once
#ifndef MANEJADOR_INPUT_H
#define MANEJADOR_INPUT_H
#include <SDL.h>

class ManejadorInput {
public:
	void update();
	bool estaTeclaPresionada(SDL_Scancode key);
private:
	const Uint8* estadoTeclado;
};

#endif