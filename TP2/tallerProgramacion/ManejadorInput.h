#pragma once
#ifndef MANEJADOR_INPUT_H
#define MANEJADOR_INPUT_H
#include <SDL.h>
#include <string>

class ManejadorInput {
public:
	static ManejadorInput* getInstance();
	void update();
	bool estaTeclaPresionada(SDL_Scancode key);
private:
	const Uint8* estadoTeclado;
	ManejadorInput();
	~ManejadorInput();
	static ManejadorInput* instance;
};

#endif