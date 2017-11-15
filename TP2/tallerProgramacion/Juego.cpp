#include "Juego.h"
#include <iostream>
#include "Carro.h"
#include "MapaView.h"

bool Juego::iniciarJuego(int cantidadJugadoresMaxima) {
	this->cantidadJugadores = 0;
	this->cantidadJugadoresMaxima = cantidadJugadoresMaxima;
	this->iniciarEscenario();
	this->procesarMapa();
	this->t_gameLoop = std::thread(&Juego::gameLoop, this);
	return true;
}

void Juego::procesarMapa() {
	int longitudTotal = 0;
	if (this->mapa != NULL) {
		std::vector<Segmento*> segmentos(this->mapa->getLongitudTotal());
		for (int i = 0; i < this->mapa->getTramosDelMapa().size(); i++) {
			Tramo* unTramo = this->mapa->getTramosDelMapa()[i];
			if (unTramo->getTipoTramo() != TipoTramo::Recta) {
				int curva;
				if (unTramo->getSentidoCurva() == SentidoCurva::SCDerecha) {
					curva = -1;
				}
				else if (unTramo->getSentidoCurva() == SentidoCurva::SCIzquierda) {
					curva = 1;
				}
				for (int j = unTramo->getMetroInicio(); j < unTramo->getMetroInicio() + unTramo->getLongitud(); j++) {
					Segmento* unSegmento = new Segmento();
					unSegmento->curva = curva;
					segmentos[j] = unSegmento;
				}
			}
		}
		this->segmentos = segmentos;
	}
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

		// Se modifica la coordenada x si esta en una curva y esta acelerando
		if (unJugador->getAcelerando()) {
			Segmento* segmentoActual = this->segmentos[unJugador->getCamara()->getPosicionTarget()->getY() / ALTO_TRAMO];
			if (segmentoActual != NULL) {
				// Es una curva
				int intensidadCentrifuga = 35;
				int nuevaPosicionX;
				if (segmentoActual->curva > 0) {
					// Curva izquierda
					nuevaPosicionX = unJugador->getPosicionX() + intensidadCentrifuga;
					if (nuevaPosicionX < LIMITE_PISTA_X_DERECHA && nuevaPosicionX > LIMITE_PISTA_X_IZQUIERDA) {
						unJugador->setPosicionX(nuevaPosicionX);
					}
					nuevoEstado->estadoJugadores[i].posX = nuevaPosicionX;
				}
				else if (segmentoActual->curva < 0) {
					// Curva derecha
					nuevaPosicionX = unJugador->getPosicionX() - intensidadCentrifuga;
					if (nuevaPosicionX < LIMITE_PISTA_X_DERECHA && nuevaPosicionX > LIMITE_PISTA_X_IZQUIERDA) {
						unJugador->setPosicionX(nuevaPosicionX);
					}
					nuevoEstado->estadoJugadores[i].posX = nuevaPosicionX;
				}
			}
		}

		nuevoEstado->estadoJugadores[i].estadoAuto = unJugador->getEstado();
		nuevoEstado->estadoJugadores[i].posX = unJugador->getPosicionX();
		nuevoEstado->estadoJugadores[i].posY = unJugador->getPosicionY();
		nuevoEstado->estadoJugadores[i].posXCamara = unJugador->getCamara()->getPosicionTarget()->getX();
		nuevoEstado->estadoJugadores[i].posYCamara = unJugador->getCamara()->getPosicionTarget()->getY();
		nuevoEstado->estadoJugadores[i].velocidadX = unJugador->getVelocidad().getX();
		nuevoEstado->estadoJugadores[i].velocidadY = unJugador->getVelocidad().getY();
	}
	
	nuevoEstado->tamanio = jugadores.size();
	
	nuevoEstado->estadoEscenario.cieloX = escenario->getPosicionCielo()->getX();
	//nuevoEstado->estadoEscenario.cieloY = escenario->getPosicionCielo()->getY();
	//nuevoEstado->estadoEscenario.colinasX = escenario->getPosicionColinas()->getX();
	//nuevoEstado->estadoEscenario.colinasY = escenario->getPosicionColinas()->getY();
	return nuevoEstado;
}

void Juego::liberarModeloEstado(EstadoModeloJuego * unEstado) {
	delete unEstado;
}

EstadoInicialJuego * Juego::getEstadoJuegoInicial() {
	EstadoInicialJuego* estado = new EstadoInicialJuego();

	for (int i = 0; i < jugadores.size(); i++) {
		Jugador* unJugador = jugadores[i];
		unJugador->setPosicionY(0);
		unJugador->getCamara()->setPosicion(unJugador->getCamara()->getPosicion()->getX(), 0);
		estado->id[i] = unJugador->getId();
	}

	escenario->getPosicionCielo()->setX(0);
	escenario->getPosicionCielo()->setY(0);
	escenario->getPosicionColinas()->setX(0);
	escenario->getPosicionColinas()->setY(POS_Y_COLINAS);

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
	this->mapa = NULL;
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
	Uint32 inicio = 0;
	Uint32 fin = 0;
	Uint32 intervalo = 0;
	while (juegoActivo) {
		inicio = SDL_GetTicks();
		this->obtenerEntrada();
		/*
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
				//std::this_thread::sleep_for(std::chrono::milliseconds(1000 / Constantes::FPS)); // por ahora se comenta, esto mejora mucho la fluidez
		}
		*/
		
		this->update(1000.0 / Constantes::FPS);
		fin = SDL_GetTicks();
		intervalo = fin - inicio;
		if (intervalo > 1000 / Constantes::FPS) {
			intervalo = (1000 / Constantes::FPS) - 1;
		}
		//cout << "itnervalo: " << intervalo << endl;
		std::this_thread::sleep_for(std::chrono::milliseconds((1000 / Constantes::FPS) - intervalo));
	}
}
