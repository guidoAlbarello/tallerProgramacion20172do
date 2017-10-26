#include "MapaView.h"


MapaView::MapaView()
{
}

MapaView::MapaView(Renderer* renderer) {
	this->gRenderer = renderer;
}

MapaView::~MapaView()
{
}

void MapaView::init() {
	this->mapa = new Mapa();
}

void MapaView::render() {
	SDL_Window* window = this->gRenderer->getWindowMapa();
	SDL_Renderer* renderer = this->gRenderer->getRendererMapa();
	SDL_SetRenderDrawColor(renderer, 242, 242, 242, 255);
	SDL_RenderClear(renderer); // Clear screen
	SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);

	this->dibujarBordes(renderer);
	this->dibujarMapa(renderer);

	SDL_RenderPresent(renderer); // render window
}

bool MapaView::loadMedia() {
	return true;
}

bool MapaView::close() {
	// cleanup SDL
	SDL_DestroyRenderer(this->gRenderer->getRendererMapa());
	SDL_DestroyWindow(this->gRenderer->getWindowMapa());
	SDL_Quit();
	return false;
}

void MapaView::update() {

}

void MapaView::dibujarBordes(SDL_Renderer* renderer) {
	// Dibuja bordes del mapa
	Line lineTop = { 10, 20, 790, 20 };
	SDL_RenderDrawLine(renderer, lineTop.x1, lineTop.y1, lineTop.x2, lineTop.y2);
	Line lineBottom = { 10, 580, 790, 580 };
	SDL_RenderDrawLine(renderer, lineBottom.x1, lineBottom.y1, lineBottom.x2, lineBottom.y2);
	Line lineLeft = { 10, 20, 10, 580 };
	SDL_RenderDrawLine(renderer, lineLeft.x1, lineLeft.y1, lineLeft.x2, lineLeft.y2);
	Line lineRightMapa = { 790, 20, 790, 580 };
	SDL_RenderDrawLine(renderer, lineRightMapa.x1, lineRightMapa.y1, lineRightMapa.x2, lineRightMapa.y2);
	Line lineRight = { 700, 20, 700, 580 };
	SDL_RenderDrawLine(renderer, lineRight.x1, lineRight.y1, lineRight.x2, lineRight.y2);
}

void MapaView::dibujarMapa(SDL_Renderer* renderer) {
	Coordenada coordenadaInicio = { 200, 300 };
	Coordenada* coordenadaUltimoTramo = new Coordenada();
	coordenadaUltimoTramo->x = 200;
	coordenadaUltimoTramo->y = 300;
	Orientacion ultimaOrientacion = Orientacion::ESTE;
	int longitudTotal = 0;
	std::vector<Tramo*> tramos = this->mapa->getTramosDelMapa();
	for (std::vector<Tramo*>::iterator it = tramos.begin(); it != tramos.end(); ++it) {
		Tramo* tramoActual = *it;
		TipoTramo tipoTramo = tramoActual->getTipoTramo();
		longitudTotal += tramoActual->getLongitud();
		Line tramo;
		TramoCurvo* tramoCurvo;
		if (tipoTramo == Recta) {
			ultimaOrientacion = this->unirTramoRecto(ultimaOrientacion, coordenadaUltimoTramo->x, coordenadaUltimoTramo->y, tramoActual->getLongitud(), coordenadaUltimoTramo, renderer);
		}
		if (tipoTramo == Curva) {
			tramoCurvo = (TramoCurvo*)tramoActual;
			ultimaOrientacion = unirTramoRotado(45, tramoCurvo->getSentido(), ultimaOrientacion, coordenadaUltimoTramo->x, coordenadaUltimoTramo->y, tramoActual->getLongitud(), coordenadaUltimoTramo, renderer);
		}
	}
}

Orientacion MapaView::unirTramoRotado(int grados, SentidoCurva sentidoRotacion, Orientacion orientacionAnterior, int ultimaX, int ultimaY, int longitud, Coordenada* coordenadaUltimoTramo, SDL_Renderer* renderer) {
	Line lineaADibujar = {};
	Orientacion orientacionNueva;

	if (orientacionAnterior == Orientacion::ESTE) {
		if (sentidoRotacion == SCDerecha) {
			lineaADibujar = { ultimaX, ultimaY, (int)(ultimaX + cos(PI / 4) * longitud), (int)(ultimaY + sin(PI / 4) * longitud) };
			orientacionNueva = Orientacion::SURESTE;
		}
		if (sentidoRotacion == SCIzquierda) {
			lineaADibujar = { ultimaX, ultimaY, (int)(ultimaX + cos(PI / 4) * longitud), (int)(ultimaY - sin(PI / 4) * longitud) };
			orientacionNueva = Orientacion::NORESTE;
		}
	}
	if (orientacionAnterior == Orientacion::NORESTE) {
		if (sentidoRotacion == SCDerecha) {
			lineaADibujar = { ultimaX, ultimaY, ultimaX + longitud, ultimaY };
			orientacionNueva = Orientacion::ESTE;
		}
		if (sentidoRotacion == SCIzquierda) {
			lineaADibujar = { ultimaX, ultimaY, ultimaX, ultimaY - longitud };
			orientacionNueva = Orientacion::NORTE;
		}
	}
	if (orientacionAnterior == Orientacion::NORTE) {
		if (sentidoRotacion == SCDerecha) {
			lineaADibujar = { ultimaX, ultimaY, (int)(ultimaX + cos(PI / 4) * longitud), (int)(ultimaY - sin(PI / 4) * longitud) };
			orientacionNueva = Orientacion::NORESTE;
		}
		if (sentidoRotacion == SCIzquierda) {
			lineaADibujar = { ultimaX, ultimaY, (int)(ultimaX - cos(PI / 4) * longitud), (int)(ultimaY - sin(PI / 4) * longitud) };
			orientacionNueva = Orientacion::NOROESTE;
		}
	}
	if (orientacionAnterior == Orientacion::NOROESTE) {
		if (sentidoRotacion == SCDerecha) {
			lineaADibujar = { ultimaX, ultimaY, ultimaX, ultimaY - longitud };
			orientacionNueva = Orientacion::NORTE;
		}
		if (sentidoRotacion == SCIzquierda) {
			lineaADibujar = { ultimaX, ultimaY, ultimaX - longitud, ultimaY };
			orientacionNueva = Orientacion::OESTE;
		}
	}
	if (orientacionAnterior == Orientacion::OESTE) {
		if (sentidoRotacion == SCDerecha) {
			lineaADibujar = { ultimaX, ultimaY, (int)(ultimaX - cos(PI / 4) * longitud), (int)(ultimaY - sin(PI / 4) * longitud) };
			orientacionNueva = Orientacion::NOROESTE;
		}
		if (sentidoRotacion == SCIzquierda) {
			lineaADibujar = { ultimaX, ultimaY, (int)(ultimaX - cos(PI / 4) * longitud), (int)(ultimaY + sin(PI / 4) * longitud) };
			orientacionNueva = Orientacion::SUROESTE;
		}
	}
	if (orientacionAnterior == Orientacion::SUROESTE) {
		if (sentidoRotacion == SCDerecha) {
			lineaADibujar = { ultimaX, ultimaY, ultimaX - longitud, ultimaY };
			orientacionNueva = Orientacion::OESTE;
		}
		if (sentidoRotacion == SCIzquierda) {
			lineaADibujar = { ultimaX, ultimaY, ultimaX, ultimaY + longitud };
			orientacionNueva = Orientacion::SUR;
		}
	}
	if (orientacionAnterior == Orientacion::SUR) {
		if (sentidoRotacion == SCDerecha) {
			lineaADibujar = { ultimaX, ultimaY, (int)(ultimaX - cos(PI / 4) * longitud), (int)(ultimaY + sin(PI / 4) * longitud) };
			orientacionNueva = Orientacion::SUROESTE;
		}
		if (sentidoRotacion == SCIzquierda) {
			lineaADibujar = { ultimaX, ultimaY, (int)(ultimaX + cos(PI / 4) * longitud), (int)(ultimaY + sin(PI / 4) * longitud) };
			orientacionNueva = Orientacion::SURESTE;
		}
	}
	if (orientacionAnterior == Orientacion::SURESTE) {
		if (sentidoRotacion == SCDerecha) {
			lineaADibujar = { ultimaX, ultimaY, ultimaX, ultimaY + longitud };
			orientacionNueva = Orientacion::SUR;
		}
		if (sentidoRotacion == SCIzquierda) {
			lineaADibujar = { ultimaX, ultimaY, ultimaX + longitud, ultimaY };
			orientacionNueva = Orientacion::ESTE;
		}
	}
	coordenadaUltimoTramo->x = lineaADibujar.x2;
	coordenadaUltimoTramo->y = lineaADibujar.y2;
	SDL_RenderDrawLine(renderer, lineaADibujar.x1, lineaADibujar.y1, lineaADibujar.x2, lineaADibujar.y2);
	return orientacionNueva;
}

Orientacion MapaView::unirTramoRecto(Orientacion orientacionAnterior, int ultimaX, int ultimaY, int longitud, Coordenada* coordenadaUltimoTramo, SDL_Renderer* renderer) {
	Line lineaADibujar = {};
	Orientacion orientacionNueva;

	if (orientacionAnterior == Orientacion::ESTE) {
		lineaADibujar = { ultimaX, ultimaY, ultimaX + longitud, ultimaY };
		orientacionNueva = Orientacion::ESTE;
	}
	if (orientacionAnterior == Orientacion::NORESTE) {
		lineaADibujar = { ultimaX, ultimaY, (int)(ultimaX + cos(PI / 4) * longitud), (int)(ultimaY - sin(PI / 4) * longitud) };
		orientacionNueva = Orientacion::NORESTE;
	}
	if (orientacionAnterior == Orientacion::NORTE) {
		lineaADibujar = { ultimaX, ultimaY, ultimaX, ultimaY - longitud };
		orientacionNueva = Orientacion::NORTE;
	}
	if (orientacionAnterior == Orientacion::NOROESTE) {
		lineaADibujar = { ultimaX, ultimaY, (int)(ultimaX - cos(PI / 4) * longitud), (int)(ultimaY - sin(PI / 4) * longitud) };
		orientacionNueva = Orientacion::NOROESTE;
	}
	if (orientacionAnterior == Orientacion::OESTE) {
		lineaADibujar = { ultimaX, ultimaY, ultimaX - longitud, ultimaY };
		orientacionNueva = Orientacion::OESTE;
	}
	if (orientacionAnterior == Orientacion::SUROESTE) {
		lineaADibujar = { ultimaX, ultimaY, (int)(ultimaX - cos(PI / 4) * longitud), (int)(ultimaY + sin(PI / 4) * longitud) };
		orientacionNueva = Orientacion::SUROESTE;
	}
	if (orientacionAnterior == Orientacion::SUR) {
		lineaADibujar = { ultimaX, ultimaY, ultimaX, ultimaY + longitud };
		orientacionNueva = Orientacion::SUR;
	}
	if (orientacionAnterior == Orientacion::SURESTE) {
		lineaADibujar = { ultimaX, ultimaY, (int)(ultimaX + cos(PI / 4) * longitud), (int)(ultimaY + sin(PI / 4) * longitud) };
		orientacionNueva = Orientacion::SURESTE;
	}
	coordenadaUltimoTramo->x = lineaADibujar.x2;
	coordenadaUltimoTramo->y = lineaADibujar.y2;
	SDL_RenderDrawLine(renderer, lineaADibujar.x1, lineaADibujar.y1, lineaADibujar.x2, lineaADibujar.y2);
	return orientacionNueva;
}