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
	proyectar(p1, anchoDest, anchoPantalla, altoPantalla, -45);
	destRect.x = p1.x;
	destRect.y = p1.y;
	destRect.w = anchoDest * 3 / 2;
	destRect.h = alto * anchoDest/ancho * 3 / 2;
	if(grisar)
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
	destRect.x = p1.x;
	destRect.y = p1.y;
	destRect.w = anchoDest;
	destRect.h = alto * anchoDest / ancho;
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

	Sint16 vxPasto[4] = { -anchoPantalla, -anchoPantalla, anchoPantalla, anchoPantalla };
	Sint16 vyPasto[4] = { p1.y, p2.y, p2.y, p1.y };
	Sint16 vxBorde[4] = { p1.x - anchoInferior *1.2,p2.x - anchoSuperior*1.2,p2.x + anchoSuperior*1.2 ,p1.x + anchoInferior*1.2 };
	Sint16 vyBorde[4] = { p1.y,p2.y,p2.y,p1.y };
	
	Sint16 vxTramo[4] = { p1.x - anchoInferior ,p2.x - anchoSuperior,p2.x + anchoSuperior ,p1.x + anchoInferior };
	Sint16 vyTramo[4] = { p1.y,p2.y,p2.y,p1.y };

	if ((n / 3) % 2)
		filledPolygonRGBA(renderer, vxPasto, vyPasto, 4, 20, 170, 20, 255);
	else
		filledPolygonRGBA(renderer, vxPasto, vyPasto, 4, 5, 130, 2, 255);
	
	if ((n / 3) % 2)
		filledPolygonRGBA(renderer, vxBorde, vyBorde, 4, 210, 210, 210, 255);
	else
		filledPolygonRGBA(renderer, vxBorde, vyBorde, 4, 50, 50, 50, 255);
		if ((n / 3) % 2)
		filledPolygonRGBA(renderer, vxTramo, vyTramo, 4, 165, 165, 165, 255);
	else
		filledPolygonRGBA(renderer, vxTramo, vyTramo, 4, 180, 180, 180, 255);

}

void ManejadorDeTexturas::proyectar(Coordenada & p, int& ancho, int anchoPantalla, int altoPantalla, float x) {
	float scalado = 0;
	int anchoOriginal = ancho; 
	float offset = p.z - camara->getPosicion()->getY() + 1;
	if (offset > 0) {
		if (offset < 0.000001)
			offset = 0;
		if (offset != 0)
			scalado = 0.84 / offset;
		else
			scalado = 0;

		p.x -= camara->getPosicion()->getX() + x;
		p.y -= 300;

		p.x = (1 + scalado * p.x) * anchoPantalla / 2;
		p.y = (1 - scalado * p.y) * altoPantalla / 2;
		ancho = scalado * ancho * anchoPantalla / 2;

		if (ancho < MINIMO_ANCHO)
			ancho = MINIMO_ANCHO;
	} else {
		ancho = anchoOriginal;
		p.x -= camara->getPosicion()->getX() - x;
		p.y = altoPantalla;
	}


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

