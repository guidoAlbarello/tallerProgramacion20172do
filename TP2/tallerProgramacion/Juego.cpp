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

	this->escenario->update(tiempoDelta);

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



EstadoModeloJuego* Juego::getEstadoJuego() {
	EstadoModeloJuego* nuevoEstado = new EstadoModeloJuego();

	for (int i = 0; i < Constantes::CANT_JUGADORES_INICIAR; i++) { //solo envia  el estado de los jugadores, deberia mandar el de todas las entidades, cambiar esto cuando haya mas objetos.
		Jugador* unJugador = jugadores[i];
		nuevoEstado->estadoJugadores[i].id = unJugador->getId();
		nuevoEstado->estadoJugadores[i].conectado = unJugador->estaConectado();
		nuevoEstado->estadoJugadores[i].estadoAuto = unJugador->getEstado();
		nuevoEstado->estadoJugadores[i].posX = unJugador->getPosicionX();
		nuevoEstado->estadoJugadores[i].posY = unJugador->getPosicionY();
		nuevoEstado->estadoJugadores[i].posXCamara = unJugador->getCamara()->getPosicion()->getX();
		nuevoEstado->estadoJugadores[i].posYCamara = unJugador->getCamara()->getPosicion()->getY();
	}
	
	nuevoEstado->tamanio = Constantes::CANT_JUGADORES_INICIAR;

	nuevoEstado->estadoEscenario.cieloX = escenario->getPosicionCielo()->getX();
	nuevoEstado->estadoEscenario.cieloY = escenario->getPosicionCielo()->getY();
	nuevoEstado->estadoEscenario.colinasX = escenario->getPosicionColinas()->getX();
	nuevoEstado->estadoEscenario.colinasY = escenario->getPosicionColinas()->getY();
	return nuevoEstado;
}

void Juego::liberarModeloEstado(EstadoModeloJuego * unEstado) {
	delete unEstado;
}

EstadoInicialJuego * Juego::getEstadoJuegoInicial() {
	EstadoInicialJuego* estado = new EstadoInicialJuego();

	for (int i = 0; i < jugadores.size(); i++) {
		Jugador* unJugador = jugadores[i];
		estado->id[i] = unJugador->getId();
	}
	estado->tamanio = Constantes::CANT_JUGADORES_INICIAR; //cambiar cuando sea dinamico

	return estado;
}

Jugador* Juego::agregarJugador() {
	Jugador* nuevoJugador = new Jugador();
	nuevoJugador->setId(cantidadJugadores);
	this->cantidadJugadores++;
	this->jugadores.push_back(nuevoJugador);
	return nuevoJugador;
}



Juego::Juego() {
	this->juegoActivo = true;
	this->escenario = new Escenario();
}

void Juego::iniciarEscenario() {
	//SDL_RenderClear(renderer->getRenderer());
	//escenario->iniciar();
	//SDL_RenderPresent(renderer->getRenderer());
	//renderer->reset();
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

		while (tiempoAcumulado >= 1000 / FPS  && nLoops < MAX_SKIP_FRAMES) {
			this->update(1000 / FPS);
			tiempoAcumulado -= 1000 / FPS;
			nLoops++;
		}
	}
}
