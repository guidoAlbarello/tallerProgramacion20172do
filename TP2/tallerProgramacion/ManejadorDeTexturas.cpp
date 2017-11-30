#include "ManejadorDeTexturas.h"

ManejadorDeTexturas* ManejadorDeTexturas::instance = 0;

ManejadorDeTexturas * ManejadorDeTexturas::getInstance() {
	if (!instance) {
		instance = new ManejadorDeTexturas();
	}

	return instance;
}



ManejadorDeTexturas::ManejadorDeTexturas() {
	this->camara = new Camara();
}

void ManejadorDeTexturas::drawAnimatedSprite(std::string id, int x, int y, int ancho, int alto, int filaActual, int frameActual, int anchoPantalla, int altoPantalla, SDL_Renderer * pRenderer, SDL_RendererFlip flip, bool grisar) {
	/*float zIndex = abs(camara->getPosicion()->getY() - 1 - y);		//hacer y - h/2 en vez de - y!!!!!!!!!!
	SDL_Rect srcRect;
	SDL_Rect destRect;
	srcRect.x = ancho * frameActual;
	srcRect.y = alto * (filaActual - 1);
	srcRect.w = ancho;
	destRect.w = ancho * 3 / 2;
	srcRect.h = alto;
	destRect.h = alto * 3 / 2;
	destRect.x = (x - destRect.w / 2);
	destRect.y = -(y - destRect.h / 2 - camara->getPosicion()->getY() - 1);

	destRect.x += -camara->getPosicion()->getX() + anchoPantalla / 2;
	destRect.y += altoPantalla * 3 / 4;*/

	Coordenada p1;
	SDL_Rect destRect;
	SDL_Rect srcRect;
	srcRect.x = ancho * frameActual;
	srcRect.y = alto * (filaActual - 1);
	srcRect.w = ancho;
	srcRect.h = alto;
	int anchoDest = ancho;
	p1.x = x;
	p1.y = 0;
	p1.z = y + 330;
	proyectar(p1, anchoDest, anchoPantalla, altoPantalla, 0);


	destRect.w = 1.3*anchoDest * 3 / 2;
	destRect.h = 1.3*alto * anchoDest / ancho * 3 / 2;
	destRect.x = p1.x - destRect.w / 2;
	destRect.y = p1.y - destRect.h / 2;
	if (grisar)
		SDL_RenderCopyEx(pRenderer, texturas["autoGrisado"], &srcRect, &destRect, 0, 0, flip);
	else
		SDL_RenderCopyEx(pRenderer, texturas[id], &srcRect, &destRect, 0, 0, flip);
}

void ManejadorDeTexturas::drawStaticSprite(std::string id, int x, int y, int ancho, int alto, int anchoPantalla, SDL_Renderer* pRenderer, SDL_RendererFlip flip, float xx) {
	Coordenada p1;
	SDL_Rect destRect;
	int anchoDest = ancho;
	p1.x = x;
	p1.y = 0;
	p1.z = y;
	proyectar(p1, anchoDest, anchoPantalla, 600, xx);
	destRect.w = anchoDest;
	destRect.h = alto * anchoDest / ancho;
	destRect.x = p1.x - destRect.w / 2;
	destRect.y = p1.y - destRect.h / 2;
	SDL_RenderCopyEx(pRenderer, texturas[id], NULL, &destRect, 0, 0, flip);
}

void ManejadorDeTexturas::dibujarSprite(std::string id, int x, int y, int ancho, int alto, int anchoPantalla, SDL_Renderer* pRenderer, SDL_RendererFlip flip) {
	SDL_Rect destRect;
	SDL_Rect srcRect;
	srcRect.x = x;
	srcRect.y = y;
	srcRect.h = alto;
	srcRect.w = anchoPantalla / 2;
	destRect.x = 0;
	destRect.y = y;
	destRect.w = anchoPantalla;
	destRect.h = alto;
	SDL_RenderCopyEx(pRenderer, texturas[id], &srcRect, &destRect, 0, 0, flip);
}
void ManejadorDeTexturas::dibujarSpriteEnCoord(std::string id, int x, int y, int ancho, int alto, int anchoPantalla, SDL_Renderer* pRenderer, SDL_RendererFlip flip, int xDest, int yDest) {
	SDL_Rect destRect;
	SDL_Rect srcRect;
	srcRect.x = x;
	srcRect.y = y;
	srcRect.h = alto;
	srcRect.w = ancho;
	destRect.x = xDest;
	destRect.y = yDest;
	destRect.w = anchoPantalla;
	destRect.h = alto;
	SDL_RenderCopyEx(pRenderer, texturas[id], &srcRect, &destRect, 0, 0, flip);
}


bool ManejadorDeTexturas::load(std::string fileName, std::string id, SDL_Renderer * pRenderer) {
	SDL_Surface* pTempSurface = IMG_Load(fileName.c_str());
	if (pTempSurface == 0) {
		return false;
	}
	SDL_Texture* pTexture =
		SDL_CreateTextureFromSurface(pRenderer, pTempSurface);
	SDL_FreeSurface(pTempSurface);
	if (pTexture != 0) {
		texturas[id] = pTexture;
		return true;
	}
	return false;
}

void ManejadorDeTexturas::dibujarTramo(Segmento* unSegmento, int ancho, int anchoPantalla, int altoPantalla, SDL_Renderer* renderer, int n, float x) {
	Coordenada p1 = unSegmento->p1;
	Coordenada p2 = unSegmento->p2;
	int anchoSuperior;
	int anchoInferior;

	int ancho1 = ancho;
	int ancho2 = ancho;

	proyectar(p1, ancho1, anchoPantalla, altoPantalla, x);
	proyectar(p2, ancho2, anchoPantalla, altoPantalla, x);
	anchoSuperior = ancho2;
	anchoInferior = ancho1;

	anchoSuperior *= 0.5; // Para que no sea tan ancha la pista
	anchoInferior *= 0.5;
	float factorAnchoPianito = 1.2;
	Sint16 vxPasto[4] = { -anchoPantalla, -anchoPantalla, anchoPantalla, anchoPantalla };
	Sint16 vyPasto[4] = { p1.y, p2.y, p2.y, p1.y };
	Sint16 vxBorde[4] = { p1.x - anchoInferior * factorAnchoPianito, p2.x - anchoSuperior * factorAnchoPianito, p2.x + anchoSuperior * factorAnchoPianito, p1.x + anchoInferior * factorAnchoPianito };
	Sint16 vyBorde[4] = { p1.y,p2.y,p2.y,p1.y };
	Sint16 vxTramo[4] = { p1.x - anchoInferior, p2.x - anchoSuperior, p2.x + anchoSuperior, p1.x + anchoInferior };
	Sint16 vyTramo[4] = { p1.y,p2.y,p2.y,p1.y };

	if ((vyTramo[0] - vyTramo[1]) > 0) {
		
		if ((n) % 2)
			filledPolygonRGBA(renderer, vxPasto, vyPasto, 4, rPasto1, gPasto1, bPasto1, 255);
		else
			filledPolygonRGBA(renderer, vxPasto, vyPasto, 4, rPasto2, gPasto2, bPasto2, 255);
		
		if ((n) % 2)
			filledPolygonRGBA(renderer, vxBorde, vyBorde, 4, 240, 80, 80, 255);
		else
			filledPolygonRGBA(renderer, vxBorde, vyBorde, 4, 240, 240, 240, 255);
			
		if ((n) % 2)
			filledPolygonRGBA(renderer, vxTramo, vyTramo, 4, 200, 200,200, 255);
		else
			filledPolygonRGBA(renderer, vxTramo, vyTramo, 4, 210, 210, 210, 255);
	}
}


/* 
	Idem dibujarTramos pero solo usa un punto para proyectar.
	Dibuja los objetos que tenga a los costados un segmento. Dado el punto proyectado, se le resta/suma el ancho del objeto
	y se le resta/suma el alto del objeto para que no se pise con la pista.
   */
void ManejadorDeTexturas::dibujarObjeto(Segmento* unSegmento, int ancho, int anchoPantalla, int altoPantalla, SDL_Renderer* renderer, int n, float x) {
	Coordenada p1 = unSegmento->p1;
	int anchoInferior;
	int ancho1 = ancho;
	float scaleP1 = proyectar(p1, ancho1, anchoPantalla, altoPantalla, x);
	anchoInferior = ancho1;

	anchoInferior *= 0.5;
	float factorAnchoPianito = 1.2;

	// Dibuja objetos a los costados en caso de que haya
	float factorDistanciaObjetoPistaIzquierda = 1.3;
	float factorDistanciaObjetoPistaDerecha = 0.8;
	float factorAltoArbol = 1.5;
	float factorTamañoCartel = 0.6;
	int factorTamañoObjetos = 2700;
	if (this->objetosPorSegmento[n].size() > 0) {
		// El segmento tiene un objeto
		std::vector<ObjetoFijo*> objetos = this->objetosPorSegmento[n];
		for (int i = 0; i < objetos.size(); i++) {
			ObjetoFijo* objetoActual = objetos[i];
			if (objetoActual->getTipoObjeto() == TipoObjeto::ARBOL) {
				if (objetoActual->getPosicion() == Posicion::PDerecha) {
					SDL_Rect destRect;
					destRect.w = 50 * scaleP1 * factorTamañoObjetos;
					destRect.h = 50 * scaleP1 * factorTamañoObjetos * factorAltoArbol;
					destRect.x = p1.x + anchoInferior * factorDistanciaObjetoPistaDerecha + destRect.w;
					destRect.y = p1.y - destRect.h;
					SDL_RenderCopyEx(renderer, texturas["arbol"], NULL, &destRect, 0, 0, SDL_FLIP_NONE);
				}
				else if (objetoActual->getPosicion() == Posicion::PIzquierda) {
					SDL_Rect destRect;
					destRect.w = 50 * scaleP1 * factorTamañoObjetos;
					destRect.h = 50 * scaleP1 * factorTamañoObjetos * factorAltoArbol;
					destRect.x = p1.x - anchoInferior * factorDistanciaObjetoPistaIzquierda - destRect.w;
					destRect.y = p1.y - destRect.h;
					SDL_RenderCopyEx(renderer, texturas["arbol"], NULL, &destRect, 0, 0, SDL_FLIP_HORIZONTAL);
				}
			}
			else if (objetoActual->getTipoObjeto() == TipoObjeto::CARTEL) {
				if (objetoActual->getPosicion() == Posicion::PDerecha) {
					SDL_Rect destRect;
					destRect.w = 50 * scaleP1 * factorTamañoObjetos * factorTamañoCartel;
					destRect.h = 50 * scaleP1 * factorTamañoObjetos * factorTamañoCartel;
					destRect.x = p1.x + anchoInferior * factorDistanciaObjetoPistaDerecha + destRect.w;
					destRect.y = p1.y - destRect.h;
					SDL_RenderCopyEx(renderer, texturas["cartel_" + objetoActual->getValor()], NULL, &destRect, 0, 0, SDL_FLIP_NONE);
				}
				else if (objetoActual->getPosicion() == Posicion::PIzquierda) {
					SDL_Rect destRect;
					destRect.w = 50 * scaleP1 * factorTamañoObjetos * factorTamañoCartel;
					destRect.h = 50 * scaleP1 * factorTamañoObjetos * factorTamañoCartel;
					destRect.x = p1.x - anchoInferior * factorDistanciaObjetoPistaIzquierda - destRect.w;
					destRect.y = p1.y - destRect.h;
					SDL_RenderCopyEx(renderer, texturas["cartel_" + objetoActual->getValor()], NULL, &destRect, 0, 0, SDL_FLIP_NONE);
				}
			}
		}
	}
}

float ManejadorDeTexturas::proyectar(Coordenada & p, int& ancho, int anchoPantalla, int altoPantalla, float x) {
	float scalado = 0;
	int anchoOriginal = ancho;
	float offset = p.z - camara->getPosicion()->getY() + 1;
	if (offset > 0) {
		if (offset < 0.000001)
			offset = 0;
		if (offset != 0)
			scalado =  0.84 / offset;
		else
			scalado = 0;

		p.x -= camara->getPosicion()->getX() + x;
		p.y -= 300;

		p.x =(1 +  scalado * p.x) * anchoPantalla / 2;
		p.y = (1 - scalado * p.y) * altoPantalla / 2;
		ancho = scalado * ancho * anchoPantalla / 2;

		if (ancho < MINIMO_ANCHO)
			ancho = 0;
	}
	else {
		//ancho = anchoOriginal;
		p.x -= camara->getPosicion()->getX() - x;
		p.y = altoPantalla;
		ancho = 0; // Para no dibujar cosas que queden atras del auto
	}
	return scalado;
}

float ManejadorDeTexturas::normZIndex(float zIndex) {
	/*float normalizado;

	if (zIndex > Z_FAR + camara->getPosicion()->getY())
	normalizado = 0;
	else
	if (zIndex < Z_NEAR + camara->getPosicion()->getY())
	normalizado = (zIndex / camara->getPosicion()->getY()) / Z_FAR;
	else
	normalizado = (zIndex - Z_NEAR - camara->getPosicion()->getY()) / Z_FAR;

	return abs(normalizado);*/
	return 0;
}

ManejadorDeTexturas::~ManejadorDeTexturas() {
}

// Recibe -1 si no se especifica a que nivel cambiar, simplemente se incrementa
void ManejadorDeTexturas::cambiarNivel(int i) {
	if (i != -1) {
		this->nivel = i;
	}
	else {
		this->nivel++;
	}
	switch (nivel) {
	case 0:
		// dia
		rPasto1 = 245;
		gPasto1 = 245;
		bPasto1 = 220;
		rPasto2 = 242;
		gPasto2 = 242;
		bPasto2 = 218;
		break;
	case 1:
		// tarde
		rPasto1 = 220;
		gPasto1 = 163;
		bPasto1 = 54;
		rPasto2 = 225;
		gPasto2 = 171;
		bPasto2 = 61;
		break;
	case 2:
		// noche
		rPasto1 = 198;
		gPasto1 = 199;
		bPasto1 = 140;
		rPasto2 = 222;
		gPasto2 = 231;
		bPasto2 = 156;
		break;
	}
}