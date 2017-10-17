#include "ManejadorInput.h"

ManejadorInput* ManejadorInput::instance = 0;

ManejadorInput * ManejadorInput::getInstance() {
	if (!instance) {
		instance = new ManejadorInput();
	}

	return instance;
}

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

ManejadorInput::ManejadorInput() {

}

ManejadorInput::~ManejadorInput() {
}
