#include "Juego.h"
#include <iostream>
#include "Carro.h"

bool Juego::iniciarJuego(int cantidadJugadoresMaxima) {
	this->cantidadJugadores = 0;
	this->cantidadJugadoresMaxima = cantidadJugadoresMaxima;
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

	for (int i = 0; i < jugadores.size(); i++) { //solo envia  el estado de los jugadores, deberia mandar el de todas las entidades, cambiar esto cuando haya mas objetos.
		Jugador* unJugador = jugadores[i];
		nuevoEstado->estadoJugadores[i].id = unJugador->getId();
		nuevoEstado->estadoJugadores[i].conectado = unJugador->estaConectado();
		nuevoEstado->estadoJugadores[i].estadoAuto = unJugador->getEstado();
		nuevoEstado->estadoJugadores[i].posX = unJugador->getPosicionX();
		nuevoEstado->estadoJugadores[i].posY = unJugador->getPosicionY();
		nuevoEstado->estadoJugadores[i].posXCamara = unJugador->getCamara()->getPosicionTarget()->getX() ;
		nuevoEstado->estadoJugadores[i].posYCamara = unJugador->getCamara()->getPosicionTarget()->getY();
	}
	
	nuevoEstado->tamanio = jugadores.size();

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

	escenario->getPosicionCielo()->setX(0);
	escenario->getPosicionCielo()->setY(0);
	escenario->getPosicionColinas()->setX(0);
	escenario->getPosicionColinas()->setY(200);
	
	return estado;
}

Jugador* Juego::agregarJugador() {
	Jugador* nuevoJugador = new Jugador();
	nuevoJugador->setId(cantidadJugadores);
	nuevoJugador->setPosicion(this->cantidadJugadores * 100, 0);
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
		auto ms = std::chrono::duration_cast<std::chrono::nanoseconds>(dur).count();
		inicioIntervalo = chrono::high_resolution_clock::now();
		tiempoAcumulado +=  ms/1000000.0;
		nLoops = 0;

		while (tiempoAcumulado >= 1000.0 / Constantes::FPS  && nLoops < MAX_SKIP_FRAMES) {
			this->update(1000.0 / Constantes::FPS);
			tiempoAcumulado -= 1000.0 / Constantes::FPS;
			nLoops++;
		}


	}
}
