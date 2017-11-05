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
	bool cerrarCliente() { return this->cerrar; }
private:
	const Uint8* estadoTeclado;
	bool cerrar = false;
	ManejadorInput();
	~ManejadorInput();
	static ManejadorInput* instance;
};

#endif