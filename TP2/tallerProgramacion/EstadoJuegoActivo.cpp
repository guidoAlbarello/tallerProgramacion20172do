#include "EstadoJuegoActivo.h"
#include <iostream>

const std::string EstadoJuegoActivo::s_playID = "JUEGO_ACTIVO";
void EstadoJuegoActivo::update(ManejadorDeConexionCliente* conexionCliente) {

	if (inicializado) {
		ManejadorInput::getInstance()->update();
		conexionCliente->enviarEntrada();
		//ManejadorInput::getInstance()->setCerrar(!conexionCliente->enviarEntrada());
	}
	//updatear posicion sprites y escenario (copiar del estado de jeugo a los objetos)
	//hago el update de cada sprite enelmap
	if (estadoModeloJuego != NULL) {
		for (int i = 0; i < estadoModeloJuego->tamanio; ++i) {

			m_estadoModelo.lock();
			EstadoJugador* estado = &(estadoModeloJuego->estadoJugadores[i]);  //lockear solo esta parte, o todo el update asi updatea todo un frame. 
			m_estadoModelo.unlock();

			if (estado->id >= 0 && estado->id < 6) {
				Sprite* unSprite = spritesMap[estado->id];
				if (unSprite != NULL) {
					if (estado->vida == 3) {
						if (estado->nitroActivo)
							unSprite->setFilaActual(2);
						else
							unSprite->setFilaActual(1);
					}

					if (estado->vida == 2) {
						if (estado->nitroActivo)
							unSprite->setFilaActual(6);
						else
							unSprite->setFilaActual(3);
					}

					if (estado->vida == 1) {
						if (estado->nitroActivo)
							unSprite->setFilaActual(7);
						else
							unSprite->setFilaActual(4);
					}

					if (estado->vida == 0) {
						if (estado->nitroActivo)
							unSprite->setFilaActual(8);
						else
							unSprite->setFilaActual(5);
					}

					switch (estado->estadoAuto) {
					case EstadoAuto::DERECHO:
						if (animacion > 0)
							unSprite->setFrameActual(0);
						else
							unSprite->setFrameActual(3);
						break;
					case EstadoAuto::DOBLANDO_IZQ:
						if (animacion > 0)
							unSprite->setFrameActual(1);
						else
							unSprite->setFrameActual(4);
						break;
					case EstadoAuto::DOBLANDO_DER:
						if (animacion > 0)
							unSprite->setFrameActual(2);
						else
							unSprite->setFrameActual(5);
						break;
					}
					animacion = animacion*-1;
					unSprite->setPosicionInt(estado->posX, estado->posY);
					unSprite->setGrisar(!estado->conectado);
				}

				if (estado->id == idJugador) {
					this->camara->setPosicion(estado->posXCamara, estado->posYCamara);

					this->escenario->setPosicionCielo(estadoModeloJuego->estadoEscenario.cieloX, estadoModeloJuego->estadoEscenario.cieloY);
					this->escenario->setVelocidad(estado->velocidadY);
					this->escenario->setDistancia(estado->posY / ALTO_TRAMO);
					this->escenario->setTiempo(estado->tiempo);
					if (estado->velocidadY > 0) {
						Segmento* segmentoActual = this->mapaView->getSegmentoActual();
						Vector* posicion = this->escenario->getPosicionCielo();
						if (estado->posX + estado->velocidadX < LIMITE_PISTA_X_DERECHA && estado->posX + estado->velocidadX > LIMITE_PISTA_X_IZQUIERDA) {
							if (estado->velocidadX < 0) {
								m_estadoModelo.lock();
								this->escenario->setPosicionCielo(this->escenario->getPosicionCielo()->getX() + 3, POS_Y_CIELO);
								this->escenario->setPosicionColinas(this->escenario->getPosicionColinas()->getX() + 6, POS_Y_COLINAS);
								m_estadoModelo.unlock();
							}
							else if (estado->velocidadX > 0) {
								m_estadoModelo.lock();
								this->escenario->setPosicionCielo(this->escenario->getPosicionCielo()->getX() - 3, POS_Y_CIELO);
								this->escenario->setPosicionColinas(this->escenario->getPosicionColinas()->getX() - 6, POS_Y_COLINAS);
								m_estadoModelo.unlock();
							}
							//if (segmentoActual != NULL) {
							//	Vector* posicion = this->escenario->getPosicionCielo();
							//	if (segmentoActual->curva > 0) {
							//		m_estadoModelo.lock();
							//		this->escenario->setPosicionCielo(posicion->getX() + 2, 0);
							//		//this->escenario->setPosicionColinas(posicion->getX() + 2, 0);
							//		m_estadoModelo.unlock();
							//	}
							//	if (segmentoActual->curva < 0) {
							//		m_estadoModelo.lock();
							//		this->escenario->setPosicionCielo(posicion->getX() - 2, 0);
							//		//this->escenario->setPosicionColinas(posicion->getX() - 2, 0);
							//		m_estadoModelo.unlock();
							//	}
							//}
						}
					}
					if (estado->sonidoChoque) {
						//ManejadorAudio::getInstance()->playTrackOnce("carCrash");
					}
					else {
						//ManejadorAudio::getInstance()->pauseTrack("c");
					}
					//SONIDO DEL MOTOR
					if ((estado->nitroActivo) && (estado->velocidadY > 0)) {
						if (idSonidoMotor != "turbo1") {
							ManejadorAudio::getInstance()->pauseTrack();
							idSonidoMotor = "turbo1";
							ManejadorAudio::getInstance()->startTrack(idSonidoMotor);
						}
					}

					if ((estado->velocidadY >= LIMITE_SONIDO_MOTOR) && !(estado->nitroActivo)) {
						if (idSonidoMotor != "motor2") {
							ManejadorAudio::getInstance()->pauseTrack();
							idSonidoMotor = "motor2";
							ManejadorAudio::getInstance()->startTrack(idSonidoMotor);
						}
					}
					if ((estado->velocidadY < LIMITE_SONIDO_MOTOR) &&
						(estado->velocidadY > 0) && !(estado->nitroActivo)) {
						if (idSonidoMotor != "motor") {
							ManejadorAudio::getInstance()->pauseTrack();
							idSonidoMotor = "motor";
							ManejadorAudio::getInstance()->startTrack(idSonidoMotor);
						}
					}
					if (estado->velocidadY <= 0) {
						if (idSonidoMotor != "") {
							ManejadorAudio::getInstance()->pauseTrack();
							idSonidoMotor = "";
						}
					}
				}
			}			
		}

		//m_estadoModelo.lock();
		
		//this->escenario->setPosicionCielo(estadoModeloJuego->estadoEscenario.cieloX, 0);
		//this->escenario->setPosicionColinas(estadoModeloJuego->estadoEscenario.colinasX, 100);
		//m_estadoModelo.unlock();
	}
}

void EstadoJuegoActivo::render() {
	if (inicializado) {
		if (!estaEnPantallaTransicion) {
			SDL_SetRenderDrawColor(renderer->getRendererJuego(), 242, 242, 242, 255);
			SDL_RenderClear(this->renderer->getRendererJuego());


			this->escenario->render();

			this->mapaView->render(this->renderer);
			//mapaview.getTramoActual me da un int, luego con eso pido el segmento actual y veo si esta doblando
			//luego veo la velocidad Y del auto, si es >0... doblo el cielo


			sort(spritesVec.begin(), spritesVec.end(), [](Sprite* a, Sprite* b)->bool {
				return a->getZIndex() > b->getZIndex();
			});

			for (int i = 0; i < spritesVec.size(); i++) {
				spritesVec[i]->render(this->renderer);
			}
			SDL_RenderPresent(this->renderer->getRendererJuego());

			//this->mapaView->renderMiniMap();
		} else {
			ManejadorAudio::getInstance()->pauseTrack();
			dibujarPantallaTransicion();
			SDL_RenderPresent(this->renderer->getRendererJuego());
			std::this_thread::sleep_for(std::chrono::milliseconds(1000 * Constantes::TIEMPO_PANTALLA_TRANSICION));
			mapaView->borrarSegmentos();
			mapaView->borrarObjetos();
			mapaView->initSegmentos();
			mapaView->renderInit();
			estaEnPantallaTransicion = false;
		}
	}
}

void EstadoJuegoActivo::dibujarPantallaTransicion() {
	int OFFSET_LEFT = 30;
	int OFFSET_TOP = 80;
	int OFFSET_BOTTOM = 120;
	int OFFSET_RIGHT = 10;
	int COLS_SEPARATION = 160;
	int OFFSET_BORDE = 10;
	int OFFSET_X_JUGADOR = OFFSET_LEFT * 6 - OFFSET_BORDE;
	int OFFSET_X_ETAPA1 = OFFSET_LEFT * 11;
	int OFFSET_X_ETAPA2 = OFFSET_LEFT * 17 - OFFSET_BORDE;
	int OFFSET_X_ETAPA3 = OFFSET_LEFT * 22 - OFFSET_BORDE;
	int SCREEN_WIDTH = 800;
	int SCREEN_HEIGHT = 600;

	SDL_Color textColor = { 0, 0, 0, 0xFF };
	SDL_SetRenderDrawColor(this->renderer->getRendererJuego(), 128, 128, 128, 255);

	Ltexture* gJugador = new Ltexture(this->renderer->getRendererJuego());
	if (!gJugador->loadFromRenderedText("Jugador", textColor)) {
		Logger::getInstance()->log(Error, "Ocurrio un error al iniciar la textura Jugador");
	}

	Ltexture* gEtapa1 = new Ltexture(this->renderer->getRendererJuego());
	if (!gEtapa1->loadFromRenderedText("Etapa 1", textColor)) {
		Logger::getInstance()->log(Error, "Ocurrio un error al iniciar la textura Etapa1");
	}

	Ltexture* gEtapa2 = new Ltexture(this->renderer->getRendererJuego());
	if (!gEtapa2->loadFromRenderedText("Etapa 2", textColor)) {
		Logger::getInstance()->log(Error, "Ocurrio un error al iniciar la textura Etapa2");
	}

	Ltexture* gEtapa3 = new Ltexture(this->renderer->getRendererJuego());
	if (!gEtapa3->loadFromRenderedText("Etapa 3", textColor)) {
		Logger::getInstance()->log(Error, "Ocurrio un error al iniciar la textura Etapa3");
	}

	Ltexture* gTotal = new Ltexture(this->renderer->getRendererJuego());
	if (!gTotal->loadFromRenderedText("Total", textColor)) {
		Logger::getInstance()->log(Error, "Ocurrio un error al iniciar la textura Total");
	}

	gJugador->render(OFFSET_LEFT, OFFSET_TOP);
	gEtapa1->render(OFFSET_LEFT + COLS_SEPARATION, OFFSET_TOP);
	gEtapa2->render(OFFSET_LEFT + 2 * COLS_SEPARATION, OFFSET_TOP);
	gEtapa3->render(OFFSET_LEFT + 3 * COLS_SEPARATION, OFFSET_TOP);
	gTotal->render(OFFSET_LEFT + 4 * COLS_SEPARATION, OFFSET_TOP);

	// Bordes
	Line lineTop = { OFFSET_LEFT - OFFSET_BORDE, OFFSET_TOP - OFFSET_BORDE, SCREEN_WIDTH - OFFSET_RIGHT, OFFSET_TOP - OFFSET_BORDE };
	SDL_RenderDrawLine(this->renderer->getRendererJuego(), lineTop.x1, lineTop.y1, lineTop.x2, lineTop.y2);

	Line lineBottom = { OFFSET_LEFT - OFFSET_BORDE, SCREEN_HEIGHT - OFFSET_BOTTOM, SCREEN_WIDTH - OFFSET_RIGHT, SCREEN_HEIGHT - OFFSET_BOTTOM };
	SDL_RenderDrawLine(this->renderer->getRendererJuego(), lineBottom.x1, lineBottom.y1, lineBottom.x2, lineBottom.y2);

	Line lineLeft = { OFFSET_LEFT - OFFSET_BORDE, OFFSET_TOP - OFFSET_BORDE, OFFSET_LEFT - OFFSET_BORDE, SCREEN_HEIGHT - OFFSET_BOTTOM };
	SDL_RenderDrawLine(this->renderer->getRendererJuego(), lineLeft.x1, lineLeft.y1, lineLeft.x2, lineLeft.y2);

	Line lineRight = { SCREEN_WIDTH - OFFSET_RIGHT, OFFSET_TOP - OFFSET_BORDE, SCREEN_WIDTH - OFFSET_RIGHT, SCREEN_HEIGHT - OFFSET_BOTTOM };
	SDL_RenderDrawLine(this->renderer->getRendererJuego(), lineRight.x1, lineRight.y1, lineRight.x2, lineRight.y2);

	Line lineJugador = { OFFSET_X_JUGADOR, OFFSET_TOP - OFFSET_BORDE, OFFSET_X_JUGADOR, SCREEN_HEIGHT - OFFSET_BOTTOM };
	SDL_RenderDrawLine(this->renderer->getRendererJuego(), lineJugador.x1, lineJugador.y1, lineJugador.x2, lineJugador.y2);

	Line lineEtapa1 = { OFFSET_X_ETAPA1, OFFSET_TOP - OFFSET_BORDE, OFFSET_X_ETAPA1, SCREEN_HEIGHT - OFFSET_BOTTOM };
	SDL_RenderDrawLine(this->renderer->getRendererJuego(), lineEtapa1.x1, lineEtapa1.y1, lineEtapa1.x2, lineEtapa1.y2);

	Line lineEtapa2 = { OFFSET_X_ETAPA2, OFFSET_TOP - OFFSET_BORDE, OFFSET_X_ETAPA2, SCREEN_HEIGHT - OFFSET_BOTTOM };
	SDL_RenderDrawLine(this->renderer->getRendererJuego(), lineEtapa2.x1, lineEtapa2.y1, lineEtapa2.x2, lineEtapa2.y2);

	Line lineEtapa3 = { OFFSET_X_ETAPA3, OFFSET_TOP - OFFSET_BORDE, OFFSET_X_ETAPA3, SCREEN_HEIGHT - OFFSET_BOTTOM };
	SDL_RenderDrawLine(this->renderer->getRendererJuego(), lineEtapa3.x1, lineEtapa3.y1, lineEtapa3.x2, lineEtapa3.y2);

	for (unsigned int i = 1; i <= this->cantJugadores; i++) {
		int puntos = estadoModeloJuego->estadoJugadores[i - 1].puntos;
		string nombreUsuario = obtenerNombreUsuarioPorId(estadoModeloJuego->estadoJugadores[i - 1].id, nombresJugadores);
		float tiempo = estadoModeloJuego->estadoJugadores[i - 1].tiempo;

		switch (this->mapaView->getNivel() - 1) {
		case 0:
			this->puntajesEtapa1[estadoModeloJuego->estadoJugadores[i - 1].id] = puntos;
			this->puntajesTotal[estadoModeloJuego->estadoJugadores[i - 1].id] += puntos;
			break;
		case 1:
			this->puntajesEtapa2[estadoModeloJuego->estadoJugadores[i - 1].id] = puntos;
			this->puntajesTotal[estadoModeloJuego->estadoJugadores[i - 1].id] += puntos;
			break;
		case 2:
			this->puntajesEtapa3[estadoModeloJuego->estadoJugadores[i - 1].id] = puntos;
			this->puntajesTotal[estadoModeloJuego->estadoJugadores[i - 1].id] += puntos;
			break;
		}

		Ltexture* gNombreUsuario = new Ltexture(this->renderer->getRendererJuego());
		if (!gNombreUsuario->loadFromRenderedText(nombreUsuario, textColor)) {
			Logger::getInstance()->log(Error, "Ocurrio un error al iniciar la textura nombreUsuario");
		}

		Ltexture* gPuntajeEtapa1 = new Ltexture(this->renderer->getRendererJuego());
		if (!gPuntajeEtapa1->loadFromRenderedText(to_string(puntajesEtapa1[estadoModeloJuego->estadoJugadores[i - 1].id]), textColor)) {
			Logger::getInstance()->log(Error, "Ocurrio un error al iniciar la textura PuntajeEtapa1");
		}

		Ltexture* gPuntajeEtapa2 = new Ltexture(this->renderer->getRendererJuego());
		if (!gPuntajeEtapa2->loadFromRenderedText(to_string(puntajesEtapa2[estadoModeloJuego->estadoJugadores[i - 1].id]), textColor)) {
			Logger::getInstance()->log(Error, "Ocurrio un error al iniciar la textura PuntajeEtapa2");
		}

		Ltexture* gPuntajeEtapa3 = new Ltexture(this->renderer->getRendererJuego());
		if (!gPuntajeEtapa3->loadFromRenderedText(to_string(puntajesEtapa3[estadoModeloJuego->estadoJugadores[i - 1].id]), textColor)) {
			Logger::getInstance()->log(Error, "Ocurrio un error al iniciar la textura PuntajeEtapa3");
		}
		
		Ltexture* gPuntajeTotal = new Ltexture(this->renderer->getRendererJuego());
		if (!gPuntajeTotal->loadFromRenderedText(to_string(puntajesTotal[estadoModeloJuego->estadoJugadores[i - 1].id]), textColor)) {
			Logger::getInstance()->log(Error, "Ocurrio un error al iniciar la textura PuntajeTotal");
		}

		gNombreUsuario->render(OFFSET_LEFT, OFFSET_TOP * i + OFFSET_TOP);
		gPuntajeEtapa1->render(OFFSET_LEFT + COLS_SEPARATION, OFFSET_TOP * i + OFFSET_TOP);
		gPuntajeEtapa2->render(OFFSET_LEFT + 2 * COLS_SEPARATION, OFFSET_TOP * i + OFFSET_TOP);
		gPuntajeEtapa3->render(OFFSET_LEFT + 3 * COLS_SEPARATION, OFFSET_TOP * i + OFFSET_TOP);
		gPuntajeTotal->render(OFFSET_LEFT + 4 * COLS_SEPARATION, OFFSET_TOP * i + OFFSET_TOP);
	}
}

string EstadoJuegoActivo::obtenerNombreUsuarioPorId(int id, map<int, string> nombresJugadores) {
	return nombresJugadores[id];
}

bool EstadoJuegoActivo::onEnter(Renderer* renderer) {
	ManejadorAudio::getInstance()->pauseTrack(); // Se pausa musica login
	this->escenario = new Escenario(renderer);
	this->escenario->iniciar();
	this->escenario->setNivel(nivel);
	this->mapaView = new MapaView(renderer);
	mapaView->setNivel(nivel);
	ManejadorDeTexturas::getInstance()->cambiarNivel(nivel);
	this->mapaView->init();
	this->camara = new Camara();
	ManejadorDeTexturas::getInstance()->setCamara(camara);
	this->initPuntajes();
	//ManejadorAudio::getInstance()->startTrack("motor2"); // Se pausa musica login
	idSonidoMotor = "motor";
	return true;
}

void EstadoJuegoActivo::initPuntajes() {
	for (int i = 0; i < 4; i++) { // FEOO
		this->puntajesEtapa1[i] = 0;
		this->puntajesEtapa2[i] = 0;
		this->puntajesEtapa3[i] = 0;
		this->puntajesTotal[i] = 0;
	}
}

bool EstadoJuegoActivo::onExit() {
	limpiarMapa();
	limpiarObjetos();
	return true;
}

void EstadoJuegoActivo::recieveInput(void * param) {
	m_estadoModelo.lock();
	if (estadoModeloJuego != NULL)
		free(estadoModeloJuego);
	estadoModeloJuego = new EstadoModeloJuego();
	memcpy(estadoModeloJuego, param, sizeof(EstadoModeloJuego));
	m_estadoModelo.unlock();
}

void EstadoJuegoActivo::cambiarNivel() {
	if (!estaEnPantallaTransicion) {
		if (this->nivel < 3) {
			estaEnPantallaTransicion = true;
			this->mapaView->cambiarNivel();
			this->escenario->cambiarNivel();
			this->nivel++;
			ManejadorDeTexturas::getInstance()->cambiarNivel(-1);
		}
	}
}

void EstadoJuegoActivo::setParametro(void * param) {
	inicializarObjetos((EstadoInicialJuego*) param);//pasar tambien pos inicial de camara ?? 
	this->renderer = renderer;
	this->inicializado = true;
	idSonidoMotor = "motor";
}

void EstadoJuegoActivo::inicializarObjetos(EstadoInicialJuego* unEstado) {
	animacion = -1;
	idJugador = unEstado->idJugador;
	this->cantJugadores = unEstado->tamanio;
	// creo los sprites del map
	for (int i = 0; i < unEstado->tamanio; i++) {
		Sprite* unSprite = new Sprite();
		std::string fileName = "imagenes/player" + std::to_string(i + 1) + ".png";//el path de la imagen o el nombre deberia venir el struct
		unSprite->setAnchoYAlto(82, 43); //cambiarlo a los alores q son 
		unSprite->setPosicionInt(0,0);
		unSprite->setId(std::to_string(unEstado->id[i]));
		unSprite->load(fileName, this->renderer->getRendererJuego());
		spritesVec.push_back(unSprite);
		spritesMap[unEstado->id[i]] = unSprite;
	}
	ManejadorDeTexturas::getInstance()->load("imagenes/tree1.png", "arbol", this->renderer->getRendererJuego());
	ManejadorDeTexturas::getInstance()->load("imagenes/billboard01.png", "cartel", this->renderer->getRendererJuego());
	ManejadorDeTexturas::getInstance()->load("imagenes/cartel_60.png", "cartel_60", this->renderer->getRendererJuego());
	ManejadorDeTexturas::getInstance()->load("imagenes/cartel_80.png", "cartel_80", this->renderer->getRendererJuego());
	ManejadorDeTexturas::getInstance()->load("imagenes/cartel_100.png", "cartel_100", this->renderer->getRendererJuego());
	ManejadorDeTexturas::getInstance()->load("imagenes/cartel_120.png", "cartel_120", this->renderer->getRendererJuego()); 
	ManejadorDeTexturas::getInstance()->load("imagenes/column.png", "8", this->renderer->getRendererJuego());
	ManejadorDeTexturas::getInstance()->load("imagenes/playerGrisado.png", "autoGrisado", this->renderer->getRendererJuego());
}

void EstadoJuegoActivo::limpiarMapa() {
	delete this->mapaView;
}

void EstadoJuegoActivo::limpiarObjetos() {
	//liberar memoria objetos
	spritesVec.clear();
	spritesMap.clear();
}

EstadoJuegoActivo::~EstadoJuegoActivo() {
	delete estadoModeloJuego;
	delete mapaView;
	delete escenario;
	delete camara;
	spritesMap.clear();
	spritesVec.clear();
	nombresJugadores.clear();
	puntajesEtapa1.clear();
	puntajesEtapa2.clear();
	puntajesEtapa3.clear();
	puntajesTotal.clear();
}
