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

void ManejadorDeTexturas::drawAnimatedSprite(std::string id, int x, int y, int ancho, int alto, int filaActual, int frameActual, int anchoPantalla, int altoPantalla, SDL_Renderer * pRenderer, SDL_RendererFlip flip) {
	float zIndex = abs(camara->getPosicion()->getY() - 1 - y);		//hacer y - h/2 en vez de - y!!!!!!!!!!
	SDL_Rect srcRect;
	SDL_Rect destRect;
	srcRect.x = ancho * frameActual;
	srcRect.y = alto * (filaActual - 1);
	//srcRect.w = destRect.w = ancho;
	//srcRect.h = destRect.h = alto;
	//destRect.x = (x - ancho / 2);
	//destRect.y = (y - alto / 2);
	//esti para ver el auto al doble de grande
	srcRect.w = ancho;
	destRect.w = ancho * 3 / 2;
	srcRect.h = alto;
	destRect.h = alto * 3 / 2;
	destRect.x = (x - destRect.w / 2);
	destRect.y = -(y - destRect.h / 2 - camara->getPosicion()->getY() - 1);
	if (destRect.y != 0) {
		//destRect.y *= FACTOR_PERSPECTIVA;//agregar h y w 
	}
	destRect.x += -camara->getPosicion()->getX() + anchoPantalla / 2;
	destRect.y += altoPantalla * 3 / 4;
	SDL_RenderCopyEx(pRenderer, texturas[id], &srcRect, &destRect, 0, 0, flip);
}

void ManejadorDeTexturas::drawStaticSprite(std::string id, int x, int y, int ancho, int alto, int anchoPantalla, SDL_Renderer* pRenderer, SDL_RendererFlip flip) {
	float zIndex = camara->getPosicion()->getY() - 1 - y - alto / 2;
	SDL_Rect srcRect;
	SDL_Rect destRect;
	srcRect.x = 0;
	srcRect.y = 0;
	srcRect.w = destRect.w = ancho;
	srcRect.h = destRect.h = alto;
	destRect.x = (x - ancho / 2) - camara->getPosicion()->getX() + anchoPantalla / 2;
	destRect.y = (y - alto / 2);

	SDL_RenderCopyEx(pRenderer, texturas[id], &srcRect, &destRect, 0, 0, flip);
}


void ManejadorDeTexturas::dibujarSprite(std::string id, int x, int y, int ancho, int alto, int anchoPantalla, SDL_Renderer* pRenderer, SDL_RendererFlip flip) {
	SDL_Rect destRect;
	destRect.x = x;
	destRect.y = y;
	destRect.w = ancho;
	destRect.h = alto;
	SDL_RenderCopyEx(pRenderer, texturas[id], NULL, &destRect, 0, 0, flip);
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

void ManejadorDeTexturas::dibujarTramo(Segmento* unSegmento, int ancho, int anchoPantalla, int altoPantalla, SDL_Renderer* renderer, int n,float x) {
	Coordenada p1 = unSegmento->p1;
	Coordenada p2 = unSegmento->p2;
	int anchoSuperior = ancho;
	int anchoInferior = ancho;
	int ancho1 = ancho;
	int ancho2 = ancho;
	proyectar(p1, ancho1, anchoPantalla, altoPantalla,x);
	proyectar(p2, ancho2, anchoPantalla, altoPantalla,x);

	anchoSuperior = ancho2 ;
	anchoInferior = ancho1 ;

	Sint16 vxPasto[4] = { -anchoPantalla, -anchoPantalla, anchoPantalla, anchoPantalla };
	Sint16 vyPasto[4] = { p1.y, p2.y, p2.y, p1.y };
	Sint16 vxBorde[4] = { p1.x - anchoInferior *1.2,p2.x - anchoSuperior*1.2,p2.x + anchoSuperior*1.2 ,p1.x + anchoInferior*1.2 };
	Sint16 vyBorde[4] = { p1.y,p2.y,p2.y,p1.y };
	Sint16 vxTramo[4] = { p1.x - anchoInferior ,p2.x - anchoSuperior,p2.x + anchoSuperior ,p1.x + anchoInferior };
	Sint16 vyTramo[4] = { p1.y,p2.y,p2.y,p1.y };

	if ((n / 3) % 2)
		filledPolygonRGBA(renderer, vxPasto, vyPasto, 4, 20, 200, 20, 255);
	else
		filledPolygonRGBA(renderer, vxPasto, vyPasto, 4, 5, 160, 2, 255);

	if ((n/3)%2)
		filledPolygonRGBA(renderer, vxBorde, vyBorde, 4, 255, 255,255, 255);
	else
		filledPolygonRGBA(renderer, vxBorde, vyBorde, 4, 0, 0, 0, 255);
	
	if ((n / 3) % 2)
		filledPolygonRGBA(renderer, vxTramo, vyTramo, 4, 115, 115,115, 255);
	else
		filledPolygonRGBA(renderer, vxTramo, vyTramo, 4, 110, 110, 110, 255);
}

void ManejadorDeTexturas::proyectar(Coordenada & p, int& ancho, int anchoPantalla, int altoPantalla,float x) {
	float scalado = 0;
	if ((p.z - camara->getPosicion()->getY() - 1) != 0)
		scalado = abs(0.6 / (p.z - camara->getPosicion()->getY() - 1));
	else
		ancho = 0;
	p.x -= camara->getPosicion()->getX() - x;
	p.y -= 300;
	
	p.x = (1 + scalado * p.x) * anchoPantalla / 2;
	p.y = (1 - scalado * p.y ) * altoPantalla / 2;
	ancho = scalado * ancho * anchoPantalla / 2;
	
	if (ancho < MINIMO_ANCHO)
		ancho = MINIMO_ANCHO;
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

