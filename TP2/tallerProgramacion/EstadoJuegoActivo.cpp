#include "EstadoJuegoActivo.h"

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
			}

			if (estado->id == idJugador)
				this->camara->setPosicion(estado->posXCamara, estado->posYCamara);
		}

		m_estadoModelo.lock();
		this->escenario->setPosicionCielo(estadoModeloJuego->estadoEscenario.cieloX, estadoModeloJuego->estadoEscenario.cieloY);
		this->escenario->setPosicionColinas(estadoModeloJuego->estadoEscenario.colinasX, estadoModeloJuego->estadoEscenario.colinasY);
		m_estadoModelo.unlock();
	}
}

void EstadoJuegoActivo::render() {
	if (inicializado) {
		this->escenario->render();
		this->mapaView->render();

		sort(spritesVec.begin(), spritesVec.end(), [](Sprite* a, Sprite* b)->bool {
			return a->getZIndex() > b->getZIndex();
		});

		for (int i = 0; i < spritesVec.size(); i++) {
			spritesVec[i]->render(this->renderer);
		}
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
	// creo los sprites del map
	for (int i = 0; i < unEstado->tamanio; i++) {
		Sprite* unSprite = new Sprite();
		std::string fileName = "imagenes/player" + std::to_string(i + 1) + ".png";//el path de la imagen o el nombre deberia venir el struct
		unSprite->setAnchoYAlto(80, 40); //cambiarlo a los alores q son 
		unSprite->setPosicionInt(-100,0);
		unSprite->setId(std::to_string(unEstado->id[i]));
		unSprite->load(fileName, this->renderer->getRendererJuego());
		spritesVec.push_back(unSprite);
		spritesMap[unEstado->id[i]] = unSprite;
	}
}

void EstadoJuegoActivo::limpiarMapa() {
	delete this->mapaView;
}

void EstadoJuegoActivo::limpiarObjetos() {
	//liberar memoria objetos

}
