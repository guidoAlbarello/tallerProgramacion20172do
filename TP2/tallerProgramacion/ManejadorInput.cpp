#include "ManejadorInput.h"

void ManejadorInput::update() {
	SDL_PumpEvents();
	estadoTeclado = SDL_GetKeyboardState(NULL);
}

bool ManejadorInput::estaTeclaPresionada(SDL_Scancode key) {
	if (estadoTeclado != 0) {
		if (estadoTeclado[key] == 1) {
			return true;
		} else {
			return false;
		}
	}
	return false;
}
