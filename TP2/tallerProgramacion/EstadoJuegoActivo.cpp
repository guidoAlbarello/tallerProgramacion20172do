#include "EstadoJuegoActivo.h"

const std::string EstadoJuegoActivo::s_playID = "JUEGO_ACTIVO";
void EstadoJuegoActivo::update(ManejadorDeConexionCliente* conexionCliente) {
	
	if (inicializado) {
		ManejadorInput::getInstance()->update();
		conexionCliente->enviarEntrada();
	}
	//updatear posicion sprites y escenario (copiar del estado de jeugo a los objetos)
	//hago el update de cada sprite enelmap
	for (std::map<int, Sprite*>::iterator it = sprites.begin(); it != sprites.end(); ++it) {
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

template <typename T1, typename T2>
struct mayor {
	typedef pair<int, Sprite*> type;
	bool operator ()(type const& a, type const& b) const {
		return a.second->getZIndex() > b.second->getZIndex();
	}
};

void EstadoJuegoActivo::render() {
	if (inicializado) {
		this->escenario->render();
		this->mapaView->render();
		
		//sort(sprites.begin(), sprites.end(), mayor<int, Sprite*>());

		for (std::map<int, Sprite*>::iterator it = sprites.begin(); it != sprites.end(); ++it) {
			it->second->render(renderer);
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
		sprites[i] = new Sprite();
		//sprites[i]->load("imagenes/player" + std::to_string(i+1) + ".png", this->renderer->getRendererJuego);
	}
}

void EstadoJuegoActivo::limpiarMapa() {
	delete this->mapaView;
}

void EstadoJuegoActivo::limpiarObjetos() {
	//liberar memoria objetos

}
