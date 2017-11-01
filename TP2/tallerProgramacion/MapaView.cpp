#include "MapaView.h"


MapaView::MapaView()
{
}

MapaView::MapaView(Renderer* renderer) {
	this->gRenderer = renderer;
	this->terminoDibujarMapa = false;
}

MapaView::~MapaView()
{
}

void MapaView::init() {
	this->mapa = new Mapa();
	initTramos();
	this->renderInit();
}

void MapaView::renderInit() {
	
	SDL_Window* window = this->gRenderer->getWindowMapa();
	SDL_Renderer* renderer = this->gRenderer->getRendererMapa();
	SDL_SetRenderDrawColor(renderer, 242, 242, 242, 255);
	SDL_RenderClear(renderer); // Clear screen
	SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);

	// Se limpian los objetos a dibujar en memoria
	this->lineasADibujarMapa.clear();
	this->objetosADibujar.clear();
	this->terminoDibujarMapa = false;

	// Se dibujan los bordes y la pista una sola vez
	this->dibujarBordes(renderer);
	this->dibujarMapa(renderer);

	SDL_RenderPresent(renderer);
}

void MapaView::renderStoredObjects() {
	SDL_Window* window = this->gRenderer->getWindowMapa();
	SDL_Renderer* renderer = this->gRenderer->getRendererMapa();
	SDL_SetRenderDrawColor(renderer, 242, 242, 242, 255);
	SDL_RenderClear(renderer); // Clear screen
	SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);

	this->dibujarBordes(renderer);

	std::vector<Line> lineasADibujarMapa = this->lineasADibujarMapa;
	std::vector<SDL_Rect*> objetosADibujar = this->objetosADibujar;

	for (std::vector<Line>::iterator it = this->lineasADibujarMapa.begin(); it != this->lineasADibujarMapa.end(); ++it) {
		Line lineaActual = *it;
		SDL_RenderDrawLine(renderer, lineaActual.x1, lineaActual.y1, lineaActual.x2, lineaActual.y2);
	}

	for (std::vector<SDL_Rect*>::iterator it = this->objetosADibujar.begin(); it != this->objetosADibujar.end(); ++it) {
		SDL_Rect* objetoActual = *it;
		SDL_RenderDrawRect(renderer, objetoActual);
	}

	SDL_RenderPresent(renderer);
}

void MapaView::renderMiniMap() {
	//SDL_Window* window = this->gRenderer->getWindowMapa();
	//SDL_Renderer* renderer = this->gRenderer->getRendererMapa();
	//SDL_SetRenderDrawColor(renderer, 242, 242, 242, 255);
	//SDL_RenderClear(renderer); // Clear screen
	//SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
	this->renderStoredObjects();

	// Aca se dibujaria solamente para la proxima entrega la posicion del auto en el minimapa y los scores

	//SDL_RenderPresent(renderer);
}

void MapaView::render(Renderer* renderer) {
	int base = getTramoActual();
	float x = 0, dx = 0;
	for (int i = 0; i < DISTANCIA_DIBUJADO; i++) {
		Segmento* unSegmento = tramos[base + i];			//agregar chequeo distancia dibujado > tamaño array
		ManejadorDeTexturas::getInstance()->dibujarTramo(unSegmento, ANCHO_TRAMO, renderer->getAnchoVentana(), renderer->getAltoVentana(), renderer->getRendererJuego(), (base + i),x);
		
		x += dx;
		dx += unSegmento->curva;
	}

	ManejadorDeTexturas::getInstance()->drawStaticSprite("8", -700, 1000, 200, 200, renderer->getAnchoVentana(), renderer->getRendererJuego(), SDL_FLIP_NONE,0);
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
	Line lineRight = { SCREEN_WIDTH - MENU_OFFSET_RIGHT, 20, SCREEN_WIDTH - MENU_OFFSET_RIGHT, 580 };
	SDL_RenderDrawLine(renderer, lineRight.x1, lineRight.y1, lineRight.x2, lineRight.y2);
}

void MapaView::dibujarMapa(SDL_Renderer* renderer) {
	Coordenada coordenadaInicio = { 200, 100 };
	Coordenada* coordenadaUltimoTramo = new Coordenada();
	coordenadaUltimoTramo->x = 200;
	coordenadaUltimoTramo->y = 300;
	Orientacion ultimaOrientacion = Orientacion::SUR;
	int longitudTotal = 0;
	std::vector<Tramo*> tramos = this->mapa->getTramosDelMapa();
	for (std::vector<Tramo*>::iterator it = tramos.begin(); it != tramos.end(); ++it) {
		Tramo* tramoActual = *it;
		TipoTramo tipoTramo = tramoActual->getTipoTramo();
		longitudTotal += tramoActual->getLongitud();
		Line tramo;
		TramoCurvo* tramoCurvo;
		std::vector<ObjetoFijo*> objetosDelMapaConUbicacion;
		if (tipoTramo == Recta) {
			this->dibujarObjetosTramo(this->mapa->getObjetosDelMapa(), ultimaOrientacion, coordenadaUltimoTramo->x, coordenadaUltimoTramo->y, tramoActual->getLongitud(), tramoActual->getMetroInicio(), coordenadaUltimoTramo, SentidoCurva::NINGUNO, renderer);
			ultimaOrientacion = this->unirTramoRecto(ultimaOrientacion, coordenadaUltimoTramo->x, coordenadaUltimoTramo->y, tramoActual->getLongitud(), coordenadaUltimoTramo, renderer);		
		} else if (tipoTramo == Curva) {
			tramoCurvo = (TramoCurvo*)tramoActual;
			this->dibujarObjetosTramo(this->mapa->getObjetosDelMapa(), ultimaOrientacion, coordenadaUltimoTramo->x, coordenadaUltimoTramo->y, tramoActual->getLongitud(), tramoActual->getMetroInicio(), coordenadaUltimoTramo, tramoCurvo->getSentido(), renderer);
			ultimaOrientacion = unirTramoRotado(tramoCurvo->getSentido(), ultimaOrientacion, coordenadaUltimoTramo->x, coordenadaUltimoTramo->y, tramoActual->getLongitud(), coordenadaUltimoTramo, renderer);
		}
	}
	this->terminoDibujarMapa = true;
}

void MapaView::dibujarObjetosTramo(std::vector<ObjetoFijo*> objetosDelMapa, Orientacion orientacionAnterior, int ultimaX, int ultimaY, int longitud, int metroInicio, Coordenada* coordenadaUltimoTramo, SentidoCurva sentidoRotacion, SDL_Renderer* renderer) {
	if (!this->terminoDibujarMapa) {
		SDL_Rect* rectObjeto = new SDL_Rect();
		std::vector<ObjetoFijo*> objetosDelMapaConUbicacion;
		for (std::vector<ObjetoFijo*>::iterator it = objetosDelMapa.begin(); it != objetosDelMapa.end(); ++it) {
			ObjetoFijo* objetoActual = *it;
			if (objetoActual->getUbicacionM() > metroInicio && objetoActual->getUbicacionM() <= metroInicio + longitud) {
				// Objeto dentro de tramo actual
				rectObjeto->w = TAMAÑO_INDICADOR_OBJETO;
				rectObjeto->h = TAMAÑO_INDICADOR_OBJETO;
				int posicionRelativaATramo = (objetoActual->getUbicacionM() - metroInicio) * ESCALA_MAPA;

				if (sentidoRotacion != SentidoCurva::NINGUNO) {
					// Si es tramo curvo
					if (orientacionAnterior == Orientacion::ESTE) {
						if (sentidoRotacion == SCDerecha) {
							rectObjeto->x = (int)(ultimaX + cos(PI / 4) * posicionRelativaATramo);
							rectObjeto->y = (int)(ultimaY + sin(PI / 4) * posicionRelativaATramo);
						}
						if (sentidoRotacion == SCIzquierda) {
							rectObjeto->x = (int)(ultimaX + cos(PI / 4) * posicionRelativaATramo);
							rectObjeto->y = (int)(ultimaY - sin(PI / 4) * posicionRelativaATramo);
						}
					}
					if (orientacionAnterior == Orientacion::NORESTE) {
						if (sentidoRotacion == SCDerecha) {
							rectObjeto->x = ultimaX + posicionRelativaATramo;
							rectObjeto->y = ultimaY;
						}
						if (sentidoRotacion == SCIzquierda) {
							rectObjeto->x = ultimaX;
							rectObjeto->y = ultimaY - posicionRelativaATramo;
						}
					}
					if (orientacionAnterior == Orientacion::NORTE) {
						if (sentidoRotacion == SCDerecha) {
							rectObjeto->x = (int)(ultimaX + cos(PI / 4) * posicionRelativaATramo);
							rectObjeto->y = (int)(ultimaY - sin(PI / 4) * posicionRelativaATramo);
						}
						if (sentidoRotacion == SCIzquierda) {
							rectObjeto->x = (int)(ultimaX - cos(PI / 4) * posicionRelativaATramo);
							rectObjeto->y = (int)(ultimaY - sin(PI / 4) * posicionRelativaATramo);
						}
					}
					if (orientacionAnterior == Orientacion::NOROESTE) {
						if (sentidoRotacion == SCDerecha) {
							rectObjeto->x = ultimaX;
							rectObjeto->y = ultimaY - posicionRelativaATramo;
						}
						if (sentidoRotacion == SCIzquierda) {
							rectObjeto->x = ultimaX - posicionRelativaATramo;
							rectObjeto->y = ultimaY;
						}
					}
					if (orientacionAnterior == Orientacion::OESTE) {
						if (sentidoRotacion == SCDerecha) {
							rectObjeto->x = (int)(ultimaX - cos(PI / 4) * posicionRelativaATramo);
							rectObjeto->y = (int)(ultimaY - sin(PI / 4) * posicionRelativaATramo);
						}
						if (sentidoRotacion == SCIzquierda) {
							rectObjeto->x = (int)(ultimaX - cos(PI / 4) * posicionRelativaATramo);
							rectObjeto->y = (int)(ultimaY + sin(PI / 4) * posicionRelativaATramo);
						}
					}
					if (orientacionAnterior == Orientacion::SUROESTE) {
						if (sentidoRotacion == SCDerecha) {
							rectObjeto->x = ultimaX - posicionRelativaATramo;
							rectObjeto->y = ultimaY;
						}
						if (sentidoRotacion == SCIzquierda) {
							rectObjeto->x = ultimaX;
							rectObjeto->y = ultimaY + posicionRelativaATramo;
						}
					}
					if (orientacionAnterior == Orientacion::SUR) {
						if (sentidoRotacion == SCDerecha) {
							rectObjeto->x = (int)(ultimaX - cos(PI / 4) * posicionRelativaATramo);
							rectObjeto->y = (int)(ultimaY + sin(PI / 4) * posicionRelativaATramo);
						}
						if (sentidoRotacion == SCIzquierda) {
							rectObjeto->x = (int)(ultimaX + cos(PI / 4) * posicionRelativaATramo);
							rectObjeto->y = (int)(ultimaY + sin(PI / 4) * posicionRelativaATramo);
						}
					}
					if (orientacionAnterior == Orientacion::SURESTE) {
						if (sentidoRotacion == SCDerecha) {
							rectObjeto->x = ultimaX;
							rectObjeto->y = ultimaY + posicionRelativaATramo;
						}
						if (sentidoRotacion == SCIzquierda) {
							rectObjeto->x = ultimaX + posicionRelativaATramo;
							rectObjeto->y = ultimaY;
						}
					}
				}
				else {
					// Si es tramo recto
					if (orientacionAnterior == Orientacion::ESTE) {
						rectObjeto->x = ultimaX + posicionRelativaATramo;
						rectObjeto->y = ultimaY;
					}
					if (orientacionAnterior == Orientacion::NORESTE) {
						rectObjeto->x = (int)(ultimaX + cos(PI / 4) * posicionRelativaATramo);
						rectObjeto->y = (int)(ultimaY - sin(PI / 4) * posicionRelativaATramo);
					}
					if (orientacionAnterior == Orientacion::NORTE) {
						rectObjeto->x = ultimaX;
						rectObjeto->y = ultimaY - posicionRelativaATramo;
					}
					if (orientacionAnterior == Orientacion::NOROESTE) {
						rectObjeto->x = (int)(ultimaX - cos(PI / 4) * posicionRelativaATramo);
						rectObjeto->y = (int)(ultimaY - sin(PI / 4) * posicionRelativaATramo);
					}
					if (orientacionAnterior == Orientacion::OESTE) {
						rectObjeto->x = ultimaX - posicionRelativaATramo;
						rectObjeto->y = ultimaY;
					}
					if (orientacionAnterior == Orientacion::SUROESTE) {
						rectObjeto->x = (int)(ultimaX - cos(PI / 4) * posicionRelativaATramo);
						rectObjeto->y = (int)(ultimaY + sin(PI / 4) * posicionRelativaATramo);
					}
					if (orientacionAnterior == Orientacion::SUR) {
						rectObjeto->x = ultimaX;
						rectObjeto->y = ultimaY + posicionRelativaATramo;
					}
					if (orientacionAnterior == Orientacion::SURESTE) {
						rectObjeto->x = (int)(ultimaX + cos(PI / 4) * posicionRelativaATramo);
						rectObjeto->y = (int)(ultimaY + sin(PI / 4) * posicionRelativaATramo);
					}
				}
				objetoActual->setXMapa(rectObjeto->x);
				objetoActual->setYMapa(rectObjeto->y);
				this->objetosADibujar.push_back(rectObjeto);
			}
			objetosDelMapaConUbicacion.push_back(objetoActual);
			SDL_RenderDrawRect(renderer, rectObjeto);
		}
		this->mapa->setObjetosDelMapa(objetosDelMapaConUbicacion);
	}
}

Orientacion MapaView::unirTramoRotado(SentidoCurva sentidoRotacion, Orientacion orientacionAnterior, int ultimaX, int ultimaY, int longitud, Coordenada* coordenadaUltimoTramo, SDL_Renderer* renderer) {
	if (!this->terminoDibujarMapa) {
		Line lineaADibujar = {};
		Orientacion orientacionNueva;
		longitud = longitud * ESCALA_MAPA;
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

		if (validarLineaDibujable(lineaADibujar)) {
			coordenadaUltimoTramo->x = lineaADibujar.x2;
			coordenadaUltimoTramo->y = lineaADibujar.y2;
			SDL_RenderDrawLine(renderer, lineaADibujar.x1, lineaADibujar.y1, lineaADibujar.x2, lineaADibujar.y2);
			this->lineasADibujarMapa.push_back(lineaADibujar);
			return orientacionNueva;
		}
		else {
			return orientacionAnterior;
		}
	}
}

Orientacion MapaView::unirTramoRecto(Orientacion orientacionAnterior, int ultimaX, int ultimaY, int longitud, Coordenada* coordenadaUltimoTramo, SDL_Renderer* renderer) {
	if (!this->terminoDibujarMapa) {
		Line lineaADibujar = {};
		Orientacion orientacionNueva;
		longitud = longitud * ESCALA_MAPA;

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

		if (validarLineaDibujable(lineaADibujar)) {
			coordenadaUltimoTramo->x = lineaADibujar.x2;
			coordenadaUltimoTramo->y = lineaADibujar.y2;
			SDL_RenderDrawLine(renderer, lineaADibujar.x1, lineaADibujar.y1, lineaADibujar.x2, lineaADibujar.y2);
			this->lineasADibujarMapa.push_back(lineaADibujar);
			return orientacionNueva;
		}
		else {
			return orientacionAnterior;
		}
	}
}

bool MapaView::validarLineaDibujable(Line lineaADibujar) {
	bool excede = lineaADibujar.x1 > SCREEN_WIDTH - MENU_OFFSET_RIGHT || lineaADibujar.x1 < 0 
		|| lineaADibujar.x2 > SCREEN_WIDTH - MENU_OFFSET_RIGHT 	|| lineaADibujar.x2 < 0 
		|| lineaADibujar.y1 > SCREEN_HEIGHT || lineaADibujar.y1 < 0 || lineaADibujar.y2 > SCREEN_HEIGHT 
		|| lineaADibujar.y2 < 0;
	if (excede) {
		ESCALA_MAPA = ESCALA_MAPA * FACTOR_DECREMENTO_ESCALA;
		this->terminoDibujarMapa = false;
		this->renderInit();
		return false;	
	}
	return true;
}

int MapaView::getTramoActual() {
	int pos = ManejadorDeTexturas::getInstance()->getCamara()->getPosicion()->getY() / 100;
	return pos;
}

void MapaView::initTramos() {
	for (int i = 0; i < CANTIDAD_SEGMENTOS; i++) {
		Segmento* unSegmento = new Segmento();
		unSegmento->p1.x = 0;
		unSegmento->p2.x = 0;
		unSegmento->p1.y = 0;
		unSegmento->p2.y = 0;
		unSegmento->p1.z = (i) * ALTO_TRAMO;
		unSegmento->p2.z = (i+1) * ALTO_TRAMO;
		unSegmento->curva = 0;
		if ( i > 100 && i < 200)
			unSegmento->curva =1;
		tramos.push_back(unSegmento);
	}
}
