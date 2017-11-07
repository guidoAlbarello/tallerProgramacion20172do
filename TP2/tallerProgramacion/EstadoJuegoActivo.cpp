#include "EstadoJuegoActivo.h"
#include <iostream>

const std::string EstadoJuegoActivo::s_playID = "JUEGO_ACTIVO";
void EstadoJuegoActivo::update(ManejadorDeConexionCliente* conexionCliente) {

	if (inicializado) {
		ManejadorInput::getInstance()->update();
		conexionCliente->enviarEntrada();
	}
	//updatear posicion sprites y escenario (copiar del estado de jeugo a los objetos)
	//hago el update de cada sprite enelmap
	if (estadoModeloJuego != NULL) {
		for (int i = 0; i < estadoModeloJuego->tamanio; ++i) {

			m_estadoModelo.lock();
			EstadoJugador* estado = &(estadoModeloJuego->estadoJugadores[i]);  //lockear solo esta parte, o todo el update asi updatea todo un frame. 
			m_estadoModelo.unlock();

			Sprite* unSprite = spritesMap[estado->id];
			if (unSprite != NULL) {
				unSprite->setFilaActual(1);//por q tiene 1 sola fila

				switch (estado->estadoAuto) {
				case EstadoAuto::DERECHO:
					unSprite->setFrameActual(0);
					break;
				case EstadoAuto::DOBLANDO_IZQ:
					unSprite->setFrameActual(1);
					break;
				case EstadoAuto::DOBLANDO_DER:
					unSprite->setFrameActual(2);
					break;
				}

				unSprite->setPosicionInt(estado->posX, estado->posY);
				unSprite->setGrisar(!estado->conectado);
			}

			if (estado->id == idJugador){
				this->camara->setPosicion(estado->posXCamara, estado->posYCamara);

				if (estado->velocidadY > 0) {
					//this->escenario->actualizar //puedo ver el 1er tramo, y en base a eso mover las nubes
					Segmento* segmentoActual = this->mapaView->getSegmentoActual();
					if (segmentoActual != NULL) {
						Vector* posicion = this->escenario->getPosicionCielo();
						if (segmentoActual->curva > 0) {
							m_estadoModelo.lock();
							this->escenario->setPosicionCielo(posicion->getX() - 2, 0);
							m_estadoModelo.unlock();
						}
						if (segmentoActual->curva < 0) {
							m_estadoModelo.lock();
							this->escenario->setPosicionCielo(posicion->getX() + 2, 0);
							m_estadoModelo.unlock();
						}
					}
				}
			}
		}

		m_estadoModelo.lock();
		
		//this->escenario->setPosicionCielo(estadoModeloJuego->estadoEscenario.cieloX, 0);
		//this->escenario->setPosicionColinas(estadoModeloJuego->estadoEscenario.colinasX, 100);
		m_estadoModelo.unlock();
	}
}

void EstadoJuegoActivo::render() {
	if (inicializado) {
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
	}
}

bool EstadoJuegoActivo::onEnter(Renderer* renderer) {
	this->escenario = new Escenario(renderer);
	this->escenario->iniciar();
	this->mapaView = new MapaView(renderer);
	this->mapaView->init();
	this->camara = new Camara();
	ManejadorDeTexturas::getInstance()->setCamara(camara);
	return true;
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

void EstadoJuegoActivo::setParametro(void * param) {
	inicializarObjetos((EstadoInicialJuego*) param);//pasar tambien pos inicial de camara ?? 
	this->renderer = renderer;
	this->inicializado = true;
}

void EstadoJuegoActivo::inicializarObjetos(EstadoInicialJuego* unEstado) {
	idJugador = unEstado->idJugador;
	this->cantJugadores = unEstado->tamanio;
	// creo los sprites del map
	for (int i = 0; i < unEstado->tamanio; i++) {
		Sprite* unSprite = new Sprite();
		std::string fileName = "imagenes/player" + std::to_string(i + 1) + ".png";//el path de la imagen o el nombre deberia venir el struct
		unSprite->setAnchoYAlto(80, 40); //cambiarlo a los alores q son 
		unSprite->setPosicionInt(0,0);
		unSprite->setId(std::to_string(unEstado->id[i]));
		unSprite->load(fileName, this->renderer->getRendererJuego());
		spritesVec.push_back(unSprite);
		spritesMap[unEstado->id[i]] = unSprite;
	}
	ManejadorDeTexturas::getInstance()->load("imagenes/tree1.png", "arbol", this->renderer->getRendererJuego());
	ManejadorDeTexturas::getInstance()->load("imagenes/billboard01.png", "cartel", this->renderer->getRendererJuego()); // TODO: conseguir el sprite de un cartel de max speed
	ManejadorDeTexturas::getInstance()->load("imagenes/column.png", "8", this->renderer->getRendererJuego());
	ManejadorDeTexturas::getInstance()->load("imagenes/playerGrisado.png", "autoGrisado", this->renderer->getRendererJuego());
}

void EstadoJuegoActivo::limpiarMapa() {
	delete this->mapaView;
}

void EstadoJuegoActivo::limpiarObjetos() {
	//liberar memoria objetos

}
