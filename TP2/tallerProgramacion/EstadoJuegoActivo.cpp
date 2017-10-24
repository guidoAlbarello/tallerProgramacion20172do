#include "EstadoJuegoActivo.h"

const std::string EstadoJuegoActivo::s_playID = "JUEGO_ACTIVO";
void EstadoJuegoActivo::update(ManejadorDeConexionCliente* conexionCliente) {

	if (inicializado) {
		ManejadorInput::getInstance()->update();
		conexionCliente->enviarEntrada();
	}
	//updatear posicion sprites y escenario (copiar del estado de jeugo a los objetos)
	//hago el update de cada sprite enelmap

	for (std::map<int, Sprite*>::iterator it = spritesMap.begin(); it != spritesMap.end(); ++it) {
		it->second->setFilaActual(0);//por q tiene 1 sola fila
		EstadoJugador estado = estadoModeloJuego.estadoJugadores[it->first];

		switch (estado.estadoAuto) {
		case EstadoAuto::DERECHO:
			it->second->setFrameActual(0);
			break;
		case EstadoAuto::DOBLANDO_IZQ:
			it->second->setFrameActual(1);
			break;
		case EstadoAuto::DOBLANDO_DER:
			it->second->setFrameActual(2);
			break;
		}

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
	inicializarMapa();
	inicializarObjetos();
	this->renderer = renderer;
	this->inicializado = true;
	return true;
}

bool EstadoJuegoActivo::onExit() {
	limpiarMapa();
	limpiarObjetos();
	return true;
}

void EstadoJuegoActivo::inicializarMapa() {
	this->mapaView = new MapaView(this->renderer);
	this->mapaView->init();

}

void EstadoJuegoActivo::inicializarObjetos() {
	// creo los sprites del map
	for (int i = 0; i < sizeof(estadoModeloJuego.estadoJugadores); i++) {
		Sprite* unSprite = new Sprite();
		std::string fileName = "imagenes/player" + std::to_string(i + 1) + ".png";
		unSprite->load(fileName, this->renderer->getRendererJuego());
		spritesVec.push_back(unSprite);
		spritesMap[i] = unSprite;
	}
}

void EstadoJuegoActivo::limpiarMapa() {
	delete this->mapaView;
}

void EstadoJuegoActivo::limpiarObjetos() {
	//liberar memoria objetos

}
