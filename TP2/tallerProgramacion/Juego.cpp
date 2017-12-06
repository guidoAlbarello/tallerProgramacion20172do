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
			Jugador* otroJugador = this->jugadores[j];
			if (otroJugador->getId() == unJugador->getId()) {
				continue;
			}
	
			if (posicionActualY / ALTO_TRAMO  < mapa[nivel]->getLongitudTotal() - 5) { // fix para que no colisione cuando se acerca a la meta
				auto time = sc::system_clock::now(); // get the current time
				auto since_epoch = time.time_since_epoch(); // get the duration since epoch
				auto millis = sc::duration_cast<sc::milliseconds>(since_epoch);
				long now = millis.count();

				if (!unJugador->estaChocado()) {
					if (hayColision(posicionAnteriorY, posicionActualY, posicionAnteriorX, posicionActualX, otroJugador)) {
						unJugador->setChocado(true);
						unJugador->chocar(otroJugador->getPosicion()->getY(), otroJugador->getVelocidad().getY());
						//cout << "Hubo colision y, y: " << unJugador->getPosicion()->getY() << endl;
					}
				}

				if (now - unJugador->getTiempoDeChocado() > 1000) {
					unJugador->setChocado(false);
				}
			}

			//fix no deje en un kilometro mas adelante que el final de la pista
			if (posicionActualY / ALTO_TRAMO > mapa[nivel]->getLongitudTotal()) {
				unJugador->setPosicionY(mapa[nivel]->getLongitudTotal() * ALTO_TRAMO);
			}
		}

		for (int j = 0; j < this->mapa[nivel]->getObjetosDelMapa().size(); j++) {
			ObjetoFijo* objeto = this->mapa[nivel]->getObjetosDelMapa()[j];
			if (hayColisionObjetoFijo(posicionAnteriorY, posicionActualY, posicionAnteriorX, posicionActualX, objeto) != 0) {
				unJugador->chocar(objeto->getUbicacionM() * ALTO_TRAMO - (ALTO_TRAMO * FACTOR_CHOQUE), 0);
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
		int factorVapuntero = vaPuntero(unJugador) ? 2 : 1;

		// Para sumar los puntos se usa la velocidad en km/h
		int velocidadEnMs = unJugador->getVelocidad().getY() * Constantes::FPS / ALTO_TRAMO;
		int velocidadEnKmh = velocidadEnMs * 3600 / 1000;
		unJugador->addPuntos(((unJugador->getPosicionY() - posicionAnteriorY) / ALTO_TRAMO) * velocidadEnKmh * factorVapuntero);

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

	if (!yaInicioTiempo) {
		this->tiempo = 0;
		yaInicioTiempo = true;
	}

	for (int i = 0; i < jugadores.size(); i++) { //solo envia  el estado de los jugadores, deberia mandar el de todas las entidades, cambiar esto cuando haya mas objetos.
		Jugador* unJugador = jugadores[i];
		if (unJugador->getId() >= 0 && unJugador->getId() < 6) { // Por si llega a venir basura
			nuevoEstado->estadoJugadores[i].id = unJugador->getId();
			nuevoEstado->estadoJugadores[i].conectado = unJugador->estaConectado();
			nuevoEstado->estadoJugadores[i].vida = unJugador->getVida();
			nuevoEstado->estadoJugadores[i].nitroActivo = unJugador->getNitroActivo();
			nuevoEstado->estadoJugadores[i].puntos = unJugador->getPuntos();
			nuevoEstado->estadoJugadores[i].tiempo = unJugador->getTiempo();
			nuevoEstado->estadoJugadores[i].sonidoChoque = unJugador->getSonidoChoque();

			if (unJugador->getCamara()->getPosicionTarget()->getY() / ALTO_TRAMO >= mapa[nivel]->getLongitudTotal()) {
				// LLego a la meta
				unJugador->setDeshabilitarMovimiento(true);
				if (!yaSumoBonusPuntero) {
					if (vaPuntero(unJugador)) {
						unJugador->addPuntos(100000);
						yaSumoBonusPuntero = true;
					}
				}
			}
			else {
				// Se modifica la coordenada x si esta en una curva y esta acelerando
				if (unJugador->getAcelerando()) {
					if (unJugador->getCamara()->getPosicionTarget()->getY() / ALTO_TRAMO < this->segmentos.size()) {
						Segmento* segmentoActual = this->segmentos[unJugador->getCamara()->getPosicionTarget()->getY() / ALTO_TRAMO];
						if (segmentoActual != NULL) {
							// Es una curva
							int nuevaPosicionX;
							if (segmentoActual->curva > 0 && unJugador->getVelocidad().getY() > Constantes::MIN_VELOCIDAD_APLICA_CENTRIFUGA) {
								// Curva izquierda
								nuevaPosicionX = unJugador->getPosicionX() + Constantes::INTENSIDAD_CENTRIFUGA;
								if (nuevaPosicionX < LIMITE_PISTA_X_DERECHA && nuevaPosicionX > LIMITE_PISTA_X_IZQUIERDA) {
									unJugador->setPosicionX(nuevaPosicionX);
								}
								nuevoEstado->estadoJugadores[i].posX = nuevaPosicionX;
							}
							else if (segmentoActual->curva < 0 && unJugador->getVelocidad().getY() > Constantes::MIN_VELOCIDAD_APLICA_CENTRIFUGA) {
								// Curva derecha
								nuevaPosicionX = unJugador->getPosicionX() - Constantes::INTENSIDAD_CENTRIFUGA;
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
		unJugador->setPosicionY(unJugador->getPosicionY());
		unJugador->getCamara()->setPosicion(unJugador->getCamara()->getPosicion()->getX(), 0);
		estado->id[i] = unJugador->getId();
	}

	escenario->getPosicionCielo()->setX(0);
	escenario->getPosicionCielo()->setY(0);
	escenario->getPosicionColinas()->setX(0);
	escenario->getPosicionColinas()->setY(POS_Y_COLINAS);
	estado->nivel = this->nivel;
	return estado;
}

Jugador* Juego::agregarJugador(string nombre) {
	Jugador* nuevoJugador = new Jugador(nombre);
	nuevoJugador->setId(cantidadJugadores);
	nuevoJugador->setPosicion(this->cantidadJugadores * 250 - ANCHO_TRAMO *1/ 4, 0);
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

int Juego::hayColision(int yDesde, int yHasta, int xDesde, int xHasta, Jugador* otroJugador) {
	int result = 0;
	//TODO, para "Y" le podria sumar la altura de el auto, para que la colision sea con la parte superior del auto
	if (yDesde > otroJugador->getPosicion()->getY() - 120) {
		return result;
	}
	if (yHasta < otroJugador->getPosicion()->getY() - 120) {
		return result;
	}
	//El punto de aclaje es 10 (no se porque, pero lo debugie)
	if (xDesde - 10 > otroJugador->getPosicion()->getX() + 110) {
		return result;
	}
	if (xHasta + 110 < otroJugador->getPosicion()->getX() - 10) {
		return result;
	}
	result = 1;
	return result;
}


// 0 si no hay colision
// != 0 si hay algun tipo de colision (delante, detras, costados)
int Juego::hayColisionObjetoFijo(int yDesde, int yHasta, int xDesde, int xHasta, ObjetoFijo* objeto2) {
	int result = 0;

	if (yDesde > (objeto2->getUbicacionM() * ALTO_TRAMO) - (ALTO_TRAMO  * FACTOR_CHOQUE)) {
		return result;
	}
	if (yHasta < (objeto2->getUbicacionM() * ALTO_TRAMO) - (ALTO_TRAMO  * FACTOR_CHOQUE)) {
		return result;
	}

	// fix para los carteles xq se ven distinto, no es la misma coordenada de colision
	boolean esCartel = false;
	if (dynamic_cast<const Cartel*>(objeto2) != NULL) {
		esCartel = true;
	}

	if (esCartel) {
		//punto de aclaje es 10, uso 30 de margen para la pista
		if ((objeto2->getPosicion() == PDerecha) &&
			((xHasta + 50) < (LIMITE_PISTA_X_DERECHA - 200))) {
			return result;
		}
		if ((objeto2->getPosicion() == PIzquierda) &&
			((xDesde - 10) > (LIMITE_PISTA_X_IZQUIERDA + 200))) {
			return result;
		}
	}
	else {
		//punto de aclaje es 10, uso 30 de margen para la pista
		if ((objeto2->getPosicion() == PDerecha) &&
			((xHasta + 50) < (LIMITE_PISTA_X_DERECHA - 30))) {
			return result;
		}
		if ((objeto2->getPosicion() == PIzquierda) &&
			((xDesde - 10) > (LIMITE_PISTA_X_IZQUIERDA + 30))) {
			return result;
		}
	}

	return 1;
}

void Juego::inicializarNivel() {
	//reiniciar posiciones jugadores 
	for (int i = 0; i < jugadores.size(); i++) {
		Jugador* unJugador = jugadores[i];
		unJugador->setPosicion(unJugador->getId() * 250 - ANCHO_TRAMO*1/4,0);
		unJugador->setDeshabilitarMovimiento(false);
		unJugador->setVida(3);
		unJugador->setChocado(false);
	}
	yaSumoBonusPuntero = false;
	tiempo = 0;
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
	int jugadoresConectados = cantidadJugadoresMaxima;
	for (int i = 0; i < jugadores.size(); i++) {
		Jugador* unJugador = jugadores[i];
		if (unJugador->getDeshabilitarMovimiento())
			jugadoresEnLaMeta++;
		if (!unJugador->estaConectado())
			jugadoresConectados--;
	}

	return jugadoresConectados != 0 ? jugadoresEnLaMeta == jugadoresConectados : false;
}
