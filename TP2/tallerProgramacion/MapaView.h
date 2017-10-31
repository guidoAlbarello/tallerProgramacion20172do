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

using namespace std;

struct Line
{
	int x1;
	int y1;
	int x2;
	int y2;
};

struct Coordenada {
	int x;
	int y;
};

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
	
private:
	Mapa* mapa;
	Renderer* gRenderer = NULL;
};

#endif