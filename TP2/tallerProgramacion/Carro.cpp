#include "Carro.h"
#include <iostream>

Carro::Carro(SDL_Renderer* renderer) : ObjetoDeJuego(renderer) {
	this->renderer = renderer;
	this->estado = DERECHO;
}

void Carro::update(Unidad delta) {
	string path = "";
	switch (this->estado) {
	case DERECHO:
		path = PATH_DERECHO;
		cout << "Cambiando a estado derecho: " << endl;
		break;
	case GIRANDO_DERECHA:
		path = PATH_GIRANDO_DERECHA;
		cout << "Cambiando a estado girando derecha: " << endl;
		break;
	case GIRANDO_IZQUIERDA:
		path = PATH_GIRANDO_IZUIERDA;
		cout << "Cambiando a estado girando izquierda: " << endl;
		break;
	default:
		cout << "Cambiando a estado default: " << endl;
		path = PATH_DERECHO;
		break;
	}
	texture->loadFromFile(path);
	texture->render(CARRO_X, CARRO_Y);
}

void Carro::setEstado(int estado) {
	this->estado = estado;
}

int Carro::getEstado() {
	return this->estado;
}
void Carro::leerEntrada(std::vector<SDL_Keycode> entradas) {
	//recibo una copia....
	while (entradas.size() > 0){
		cout << "leyendo entradas, sizee: " << entradas.size() << endl;
		SDL_Keycode key = entradas.at(entradas.size() - 1);
		entradas.pop_back();
		switch (key) {
			case SDLK_RIGHT:
				this->setEstado(GIRANDO_DERECHA);
				break;
			case SDLK_LEFT:
				this->setEstado(GIRANDO_IZQUIERDA);
				break;
			case SDLK_UP:
				this->setEstado(DERECHO);
				break;
		}
	}
	
}