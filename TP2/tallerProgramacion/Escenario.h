#pragma once
#ifndef ESCENARIO_H
#define ESCENARIO_H

#define VELOCIDAD_CIELO 0.1
#define VELOCIDAD_COLINAS 0.05
#define VELOCIDAD_COLINAS 1
#define POS_Y_CIELO 0
#define ALTO_CIELO 150
#define POS_Y_COLINAS 150
#define ALTO_COLINAS 450

#include "Sprite.h"
#include "Vector.h"
#include "Constantes.h"
#include "TableroPuntosEnJuego.h"
#include <sstream>

class Escenario {
public:
	Escenario();
	Escenario(Renderer* renderer);
	void iniciar();
	~Escenario();
	void update(Unidad deltaTiempo);
	void render();
	Vector* getPosicionCielo() { return &posicionCielo; }
	Vector* getPosicionColinas() { return &posicionColinas; }
	void setPosicionCielo(int x, int y);
	void setDistancia(long distancia);
	void setVelocidad(long velocidad);
	void setPosicionColinas(int x, int y);
	bool limiteImagenCielo() { return posicionCielo.getX() < 800; }
	bool limiteImagenColinas() { return posicionColinas.getX() < 800; }
	//bool limiteImagenCielo() { return posicionCielo.getX() < 1600; }
	//bool limiteImagenColinas() { return posicionColinas.getX() < 1600; }
	void setTiempo(long tiempo);
	void setNivel(int nivel) { 
		this->nivel = nivel; 
		cambiarCieloYColinas(); 
	}
	int getNivel() { return this->nivel; }
	void cambiarNivel();
	void cambiarCieloYColinas();
private:
	Vector posicionCielo;
	Vector posicionColinas;
	Sprite* cielo;
	Sprite* colinas;
	Sprite* pasto;
	Renderer* renderer;
	long distancia;
	long velocidad;
	TableroPuntosEnJuego* tablero;
	long tiempo;
	int nivel;
};
#endif