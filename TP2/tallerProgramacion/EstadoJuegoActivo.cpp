#include "EstadoJuegoActivo.h"

const std::string EstadoJuegoActivo::s_playID = "JUEGO_ACTIVO";
void EstadoJuegoActivo::update(ManejadorDeConexionCliente* conexionCliente) {
	
	if (inicializado) {
		ManejadorInput::getInstance()->update();
		conexionCliente->enviarEntrada();
	}
	//updatear posicion sprites y escenario (copiar del estado de jeugo a los objetos)
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
	this->escenario = new Escenario();
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
	//inicializar la estrucutra de datos de objetos


}

void EstadoJuegoActivo::limpiarMapa() {
	delete this->mapaView;
}

void EstadoJuegoActivo::limpiarObjetos() {
	//liberar memoria objetos

}
