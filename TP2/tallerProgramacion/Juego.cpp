#include "Juego.h"

bool Juego::iniciarJuego() {
	this->t_gameLoop = std::thread(&Juego::gameLoop, this);
	return true;
}

void Juego::update(Unidad tiempoDelta) {
	//update de todos los elementos del juego
	for (int i = 0; i < this->objetosDeJuego.size(); i++) {
		ObjetoDeJuego* unObjeto = this->objetosDeJuego[i];
		unObjeto->update(tiempoDelta);								//podria hacerse alguna estructura q tenga la pos de los jugadores, y solo updatear lo q esta cerca de esos jugadores en vez de todas las entidades. 
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

Juego::Juego() {
	this->juegoActivo = true;
}

void Juego::gameLoop() {	
	auto inicioIntervalo = chrono::high_resolution_clock::now();
	double tiempoAcumulado = 0;
	int nLoops;

	while (juegoActivo) {

		this->obtenerEntrada();
		auto finIntervalo = chrono::high_resolution_clock::now();
		auto dur = finIntervalo - inicioIntervalo;
		auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
		inicioIntervalo = chrono::high_resolution_clock::now();
		tiempoAcumulado += ms;
		nLoops = 0;
		while (tiempoAcumulado >= 1000/FPS  && nLoops < MAX_SKIP_FRAMES) {
			this->update(1000/FPS);
			tiempoAcumulado -= 1000/FPS;
			nLoops++;
		}

	}
}