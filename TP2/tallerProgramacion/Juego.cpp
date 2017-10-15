#include "Juego.h"
#include <iostream>
#include "Carro.h"

bool Juego::iniciarJuego() {
	this->iniciarEscenario();
	this->t_gameLoop = std::thread(&Juego::gameLoop, this);
	return true;
}

void Juego::update(Unidad tiempoDelta) {
	//update de todos los elementos del juego
	for (int i = 0; i < this->objetosDeJuego.size(); i++) {
		ObjetoDeJuego* unObjeto = this->objetosDeJuego[i];
		unObjeto->update(tiempoDelta);	//podria hacerse alguna estructura q tenga la pos de los jugadores, y solo updatear lo q esta cerca de esos jugadores en vez de todas las entidades. 
	}

	for (int i = 0; i < this->jugadores.size(); i++) {
		Jugador* unJugador = this->jugadores[i];
		unJugador->update(tiempoDelta);
	}

}

void Juego::obtenerEntrada() {
	//update entrada para todos los jugadores. 
	for (int i = 0; i < this->jugadores.size(); i++) {
		Jugador* unJugador = this->jugadores[i];
		unJugador->leerEntrada();
	}
}

void Juego::agregarObjetoDeJuego(ObjetoDeJuego* objetoDeJuego) {
	this->objetosDeJuego.push_back(objetoDeJuego);
}

std::vector<ObjetoDeJuego*> Juego::getObjetosDeJuego() {
	return this->objetosDeJuego;
}

std::vector<Jugador*> Juego::getJugadores() {
	return this->jugadores;
}

Jugador * Juego::agregarJugador() {
	
	Jugador* nuevoJugador = new Jugador(renderer->getRenderer());
	nuevoJugador->setId(cantidadJugadores);
	this->cantidadJugadores++;
	this->jugadores.push_back(nuevoJugador);
	return nuevoJugador;
}

Juego::Juego() {
	this->juegoActivo = true;
}

Juego::Juego(Renderer* renderer) {
	this->renderer = renderer;
	this->juegoActivo = true;
	this->escenario = new Escenario(renderer);
}

void Juego::iniciarEscenario() {
	SDL_RenderClear(renderer->getRenderer());
	escenario->iniciar();
	SDL_RenderPresent(renderer->getRenderer());
	//renderer->reset();
}

void Juego::gameLoop() {	
	auto inicioIntervalo = chrono::high_resolution_clock::now();
	double tiempoAcumulado = 0;
	int nLoops;
	
	/*por ahora lo creo aca.. pero en realidad lo tendria q leer del xml*/
	Carro * carro = new Carro(renderer->getRenderer());
	carro->inicializar("imagenes/player_straight.png");
	this->objetosDeJuego.push_back(carro);

	while (juegoActivo) {
		this->obtenerEntrada();
		auto finIntervalo = chrono::high_resolution_clock::now();
		auto dur = finIntervalo - inicioIntervalo;
		auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
		inicioIntervalo = chrono::high_resolution_clock::now();
		tiempoAcumulado += ms;
		nLoops = 0;
		//cout << "game loop" << endl;
		while (tiempoAcumulado >= 1000/FPS  && nLoops < MAX_SKIP_FRAMES) {
			this->update(1000/FPS);
			tiempoAcumulado -= 1000/FPS;
			nLoops++;
			//Este valor depende de los FPSs, pero a 25 fps, son 40ms por frame 
			SDL_Delay(40);
		}
		SDL_RenderPresent(renderer->getRenderer());
	}
}
