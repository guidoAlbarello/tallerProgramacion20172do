#include "ManejadorInput.h"

ManejadorInput* ManejadorInput::instance = 0;

ManejadorInput * ManejadorInput::getInstance() {
	if (!instance) {
		instance = new ManejadorInput();
	}

	return instance;
}

void ManejadorInput::update() {
	SDL_Event e;
	while (SDL_PollEvent(&e) != 0) {
		if (e.type == SDL_WINDOWEVENT) {
			if(e.window.event == SDL_WINDOWEVENT_CLOSE) 
				cerrar = true;
		}
		if (e.type == SDL_QUIT) {
			cerrar = true;
		}
	}
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
