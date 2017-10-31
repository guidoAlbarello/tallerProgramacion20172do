#pragma once
#ifndef MAPAVIEW_H
#define MAPAVIEW_H

//#include "Ltexture.h"
#include <string>
#include "Renderer.h"
#include "Constantes.h"
#include "Mapa.h"
#include "TramoCurvo.h"
#include "ManejadorDeTexturas.h"

#define PI 3.14159265
#define DISTANCIA_DIBUJADO 200
#define ANCHO_TRAMO 800
#define ALTO_TRAMO 70

using namespace std;



enum class Orientacion { ESTE = '0', NORESTE = '1', NORTE = '2', NOROESTE = '3', OESTE = '4', SUROESTE = '5', SUR = '6', SURESTE = '7' };

class MapaView
{
public:
	MapaView();
	MapaView(Renderer* renderer);
	~MapaView();
	void init();
	void renderMiniMap();
	void render(Renderer* renderer);
	bool loadMedia();
	bool close();
	void update();
	void dibujarBordes(SDL_Renderer* renderer);
	void dibujarMapa(SDL_Renderer* renderer);
	Orientacion unirTramoRotado(int grados, SentidoCurva sentidoRotacion, Orientacion orientacionAnterior, int ultimaX, int ultimaY, int longitud, Coordenada* coordenadaUltimoTramo, SDL_Renderer* renderer);
	Orientacion unirTramoRecto(Orientacion orientacionAnterior, int ultimaX, int ultimaY, int longitud, Coordenada* coordenadaUltimoTramo, SDL_Renderer* renderer);
	bool validarLineaDibujable(Line lineaADibujar);
	static constexpr const int SCREEN_WIDTH = 800;
	static constexpr const int SCREEN_HEIGHT = 600;
	static constexpr const int MENU_OFFSET_RIGHT = 100;
	static constexpr const int TAMAÑO_INDICADOR_OBJETO = 3;
	float ESCALA_MAPA = 0.75;
	float FACTOR_DECREMENTO_ESCALA = 2 / 3;
	int getTramoActual();
	void initTramos();
private:
	Mapa* mapa;
	Renderer* gRenderer = NULL;
	std::vector<Segmento*> tramos;
};

#endif