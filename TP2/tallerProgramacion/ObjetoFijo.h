#pragma once
#ifndef OBJETOFIJO_H
#define OBJETOFIJO_H

#include "ObjetoDeJuego.h"

enum Posicion { PDerecha = 0, PIzquierda = 1 };

class ObjetoFijo : public ObjetoDeJuego {
public:
	Posicion getPosicion();
	void setPosicion(Posicion posicion) { this->posicion = posicion; }
	int getUbicacionM();
	int getTramo() { return this->tramo; }
	void setTramo(int tramo) {
		this->tramo = tramo;
	}
	int getXMapa() { return this->xMapa; };
	void setXMapa(int xMapa) { this->xMapa = xMapa; }
	int getYMapa() { return this->yMapa; };
	void setYMapa(int yMapa) { this->yMapa = yMapa; }
	TipoObjeto getTipoObjeto() { return this->tipoObjeto; }
	void setTipoObjeto(TipoObjeto tipoObjeto) { this->tipoObjeto = tipoObjeto; }
protected:
	Posicion posicion;
	int ubicacionMetros;
	int tramo;
	int xMapa;
	int yMapa;
	TipoObjeto tipoObjeto;
};

#endif
