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
	this->tiempo = 0;
	return true;
}

void Juego::procesarMapa() {
	int longitudTotal = 0;
	if (this->mapa != NULL) {
		std::vector<Segmento*> segmentos(this->mapa[nivel]->getLongitudTotal());
		for (int i = 0; i < this->mapa[nivel]->getTramosDelMapa().size(); i++) {
			Tramo* unTramo = this->mapa[nivel]->getTramosDelMapa()[i];
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
	tiempo += tiempoDelta;
	this->escenario->update(tiempoDelta);

	for (int i = 0; i < this->objetosDeJuego.size(); i++) {
		ObjetoDeJuego* unObjeto = this->objetosDeJuego[i];
		unObjeto->update(tiempoDelta);	//podria hacerse alguna estructura q tenga la pos de los jugadores, y solo updatear lo q esta cerca de esos jugadores en vez de todas las entidades. 
	}

	for (int i = 0; i < this->jugadores.size(); i++) {
		Jugador* unJugador = this->jugadores[i];
		int posicionAnteriorY = unJugador->getPosicionY();
		int posicionAnteriorX = unJugador->getPosicionX();
		unJugador->update(tiempoDelta);
		int posicionActualY = unJugador->getPosicionY();
		int posicionActualX = unJugador->getPosicionX();
		//cout << "jugador z: " << unJugador->getZIndex() << ", jugador Y: " << unJugador->getPosicionY() << endl;
		
		//Si los demas jugadores estan SOLO en "jugadores", hay q hacer otro de estos for...
		for (int j = 0; j < this->jugadores.size(); j++) {
			Jugador* jugador = this->jugadores[j];
			if (jugador->getId() == unJugador->getId()) {
				break;
			}
			if (hayColision(posicionAnteriorY, posicionActualY, posicionAnteriorX, posicionActualX, jugador)) {
				unJugador->chocar(jugador->getPosicion()->getY());
				//cout << "Hubo colision y, y: " << unJugador->getPosicion()->getY() << endl;
			}
		}

		for (int j = 0; j < this->mapa[nivel]->getObjetosDelMapa().size(); j++) {
			ObjetoFijo* objeto = this->mapa[nivel]->getObjetosDelMapa()[j];
			if (hayColisionObjetoFijo(posicionAnteriorY, posicionActualY, posicionAnteriorX, posicionActualX, objeto) != 0) {
				unJugador->chocar(objeto->getUbicacionM() * 100 - (ALTO_TRAMO * 2));
					//cout << "COLISION, yAnterior: " << posicionAnteriorY << ", actual: " << posicionActualY << ", objetoM: " << objeto->getUbicacionM() << ", velocidadY: "<< unJugador->getVelocidad().getY() <<endl;
			}
		}

		//DETECCION FIN DE RECORRIDO
		/*
		int segmentoActual = posicionActualY / 100;
		if (segmentoActual >= segmentos.size()) {
		//FIN....
		//hABRIA QUE IMPEDIR QUE LA POSICION_Y llegue mas alla del limite... x los puntos, o dejarlo, y
		//suma mas puntos quien vaya mas rapido
		}
		*/

		//Ojo cuando choca, la velocidad queda en cero, entonces en ese frame no suma nada
		int factorVapunterto = vaPuntero(unJugador) ? 2 : 1;
		unJugador->addPuntos(((unJugador->getPosicionY() - posicionAnteriorY) / 100) * unJugador->getVelocidad().getY() * factorVapunterto);
		//cout << "puntos: " << unJugador->getPuntos() << ", y: "<< unJugador->getPosicionY() / 100 << ", velocidad: " << unJugador->getVelocidad().getY() << ", puntero: " << vaPuntero(unJugador) <<endl;
		unJugador->setTiempo(tiempo / 1000);
	}

}

bool Juego::vaPuntero(Jugador * unJugador) {
	bool result = true;
	for (int i = 1; i < this->jugadores.size(); i++) {
		Jugador* otroJugador = this->jugadores[i];
		if (otroJugador->getId() == unJugador->getId()) {
			continue;
		}
		if (otroJugador->getPosicionY() >= unJugador->getPosicionY()) {
			result = false;
			break;
		}
	}
	return result;
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
		nuevoEstado->estadoJugadores[i].vida = unJugador->getVida();
		nuevoEstado->estadoJugadores[i].puntos = unJugador->getPuntos();
		nuevoEstado->estadoJugadores[i].tiempo = unJugador->getTiempo();

		if (unJugador->getCamara()->getPosicionTarget()->getY() / ALTO_TRAMO >= mapa[nivel]->getLongitudTotal()) {
			// LLego a la meta
			unJugador->setDeshabilitarMovimiento(true);
		}
		else {
			// Se modifica la coordenada x si esta en una curva y esta acelerando
			if (unJugador->getAcelerando()) {
				if (unJugador->getCamara()->getPosicionTarget()->getY() / ALTO_TRAMO < this->segmentos.size()) {
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

Jugador* Juego::agregarJugador(string nombre) {
	Jugador* nuevoJugador = new Jugador(nombre);
	nuevoJugador->setId(cantidadJugadores);
	nuevoJugador->setPosicion(this->cantidadJugadores * 100, 0);
	this->cantidadJugadores++;
	this->jugadores.push_back(nuevoJugador);
	return nuevoJugador;
}

Juego::Juego() {
	this->juegoActivo = true;
	this->escenario = new Escenario();
	this->mapa[0] = NULL;
	this->mapa[1] = NULL;
	this->mapa[2] = NULL;
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

int Juego::hayColision(int yDesde, int yHasta, int xDesde, int xHasta, Jugador* objeto2) {
	int result = 0;
	//TODO, para "Y" le podria sumar la altura de el auto, para que la colision sea con la parte superior del auto
	if (yDesde  > objeto2->getPosicion()->getY() - 30) {
		return result;
	}
	if (yHasta < objeto2->getPosicion()->getY() - 30) {
		return result;
	}
	//El punto de aclaje es 10 (no se porque, pero lo debugie)
	if (xDesde - 10 > objeto2->getPosicion()->getX() + 50) {
		return result;
	}
	if (xHasta + 50 < objeto2->getPosicion()->getX() - 10) {
		return result;
	}
	result = 1;

	return result;
}


// 0 si no hay colision
// != 0 si hay algun tipo de colision (delante, detras, costados)
int Juego::hayColisionObjetoFijo(int yDesde, int yHasta, int xDesde, int xHasta, ObjetoFijo* objeto2) {
	int result = 0;
	if (yDesde > (objeto2->getUbicacionM() * 100) - (ALTO_TRAMO * 2)) {
		return result;
	}
	if (yHasta < (objeto2->getUbicacionM() * 100) - (ALTO_TRAMO * 2)) {
		return result;
	}
	//punto de aclaje es 10, uso 30 de margen para la pista
	if ((objeto2->getPosicion() == PDerecha) &&
		((xHasta + 50) < (LIMITE_PISTA_X_DERECHA - 30))) {
		return result;
	}
	if ((objeto2->getPosicion() == PIzquierda) &&
		((xDesde - 10) > (LIMITE_PISTA_X_IZQUIERDA + 30))) {
		return result;
	}
	result = 1;

	return result;
}

void Juego::inicializarNivel() {
	//reiniciar posiciones jugadores 
	for (int i = 0; i < jugadores.size(); i++) {
		Jugador* unJugador = jugadores[i];
		unJugador->setPosicion(unJugador->getId() * 100,0);
		unJugador->setDeshabilitarMovimiento(false);
	}
	
	//aumentar nivel
	if (nivel < 2) {
		nivel++;
		this->iniciarEscenario();
		this->procesarMapa();
	} else
		juegoTerminado = true;
}

bool Juego::terminoNivel() {
	int jugadoresEnLaMeta = 0;
	for (int i = 0; i < jugadores.size(); i++) {
		Jugador* unJugador = jugadores[i];
		if (unJugador->getDeshabilitarMovimiento())
			jugadoresEnLaMeta++;
	}

	return jugadoresEnLaMeta == cantidadJugadoresMaxima;
}
