#pragma once
#ifndef MANEJADOR_DE_TEXTURAS_H
#define MANEJADOR_DE_TEXTURAS_H

#define MINIMO_ANCHO 4


#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <map>
#include "Camara.h"
#include <stdlib.h>  
#include <math.h>
#include "SDL2_gfxPrimitives.h"
#include "Constantes.h"
#include <vector>
#include "ObjetoFijo.h"

class ManejadorDeTexturas {
public:
	static ManejadorDeTexturas* getInstance();
	void drawAnimatedSprite(std::string id, int x, int y, int ancho, int alto, int filaActual, int frameActual, int anchoPantalla, int altoPantalla,
			SDL_Renderer *pRenderer, SDL_RendererFlip flip, bool grisar);
	void drawStaticSprite(std::string id, int x, int y, int ancho, int alto, int anchoPantalla, SDL_Renderer* pRenderer, SDL_RendererFlip flip, float xx);
	void dibujarSprite(std::string id, int x, int y, int ancho, int alto, int anchoPantalla, SDL_Renderer* pRenderer, SDL_RendererFlip flip);
	void dibujarSpriteEnCoord(std::string id, int x, int y, int ancho, int alto, int anchoPantalla, SDL_Renderer* pRenderer, SDL_RendererFlip flip, int xDest, int yDest);
	bool load(std::string fileName, std::string	id, SDL_Renderer* pRenderer);
	void setCamara(Camara* unaCamara) { this->camara = unaCamara; }
	void dibujarTramo(Segmento* unSegmento,int ancho, int anchoPantalla, int altoPantalla, SDL_Renderer* renderer, int n, float x);
	void dibujarObjeto(Segmento* unSegmento, int ancho, int anchoPantalla, int altoPantalla, SDL_Renderer* renderer, int n, float x);
	Camara* getCamara() { return camara; }
	void setObjetosPorSegmento(std::vector<std::vector<ObjetoFijo*>> objetosPorSegmento) { this->objetosPorSegmento = objetosPorSegmento; }
	std::vector<std::vector<ObjetoFijo*>> getObjetosPorSegmento() { return this->objetosPorSegmento; }
	void cambiarNivel(int nivel);
	void setNivel(int nivel) { this->nivel = nivel; }
private:
	Camara* camara;
	float normZIndex(float zIndex);
	static ManejadorDeTexturas* instance;
	ManejadorDeTexturas();
	~ManejadorDeTexturas();
	float proyectar(Coordenada& p, int& ancho, int anchoPantalla, int altoPantalla,float x);
	std::map<std::string, SDL_Texture*> texturas;
	std::vector<std::vector<ObjetoFijo*>> objetosPorSegmento;
	int nivel = 0;
	// Colores iniciales pasto verde
	int rPasto1 = 20;
	int gPasto1 = 170;
	int bPasto1 = 20;
	int rPasto2 = 5;
	int gPasto2 = 130;
	int bPasto2 = 2;
};

#endif