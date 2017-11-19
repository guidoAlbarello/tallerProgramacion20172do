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
#define DISTANCIA_DIBUJADO 50
#define DISTANCIA_DIBUJADO_OBJETOS 50
#define ALTO_TRAMO 100
#define INTENSIDAD_CURVAS 0.8
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
	void renderInit();
	void renderStoredObjects();
	bool loadMedia();
	bool close();
	void update();
	Segmento* getSegmentoActual();
	void dibujarBordes(SDL_Renderer* renderer);
	void dibujarMapa(SDL_Renderer* renderer);
	Orientacion unirTramoRotado(SentidoCurva sentidoRotacion, Orientacion orientacionAnterior, int ultimaX, int ultimaY, int longitud, Coordenada* coordenadaUltimoTramo, SDL_Renderer* renderer);
	Orientacion unirTramoRecto(Orientacion orientacionAnterior, int ultimaX, int ultimaY, int longitud, Coordenada* coordenadaUltimoTramo, SDL_Renderer* renderer);
	void dibujarObjetosTramo(std::vector<ObjetoFijo*> objetosDelMapa, Orientacion orientacionAnterior, int ultimaX, int ultimaY, int longitud, int metroInicio, Coordenada* coordenadaUltimoTramo, SentidoCurva sentidoRotacion, SDL_Renderer* renderer);
	bool validarLineaDibujable(Line lineaADibujar);
	static constexpr const int SCREEN_WIDTH = 800;
	static constexpr const int SCREEN_HEIGHT = 600;
	static constexpr const int MENU_OFFSET_RIGHT = 100;
	static constexpr const int TAMAÑO_INDICADOR_OBJETO = 3;
	float ESCALA_MAPA = 0.75;
	float FACTOR_DECREMENTO_ESCALA = 0.66;
	
private:
	int getTramoActual();
	Mapa* mapa;
	Renderer* gRenderer = NULL;
	std::vector<Segmento*> segmentos;
	std::vector<Line> lineasADibujarMapa;
	std::vector<SDL_Rect*> objetosADibujar;
	void initSegmentos();
	bool terminoDibujarMapa;
	int metroActualAuto;
	std::vector<std::vector<ObjetoFijo*>> objetosPorSegmento;
	
};

#endif