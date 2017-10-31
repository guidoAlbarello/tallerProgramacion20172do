#pragma once
#ifndef TRAMO_H
#define TRAMO_H
#include "Tramo.h"
#include "ObjetoFijo.h"

enum TipoTramo { Recta = 0, Curva = 1 };

class Tramo {
public:
	int getLongitud();
	void setLongitud(int longitud) {
		this->longitud = longitud;
	}
	TipoTramo getTipoTramo();
	void setId(int newId) {	this->id = newId; }
	int getId() { return this->id; }
	void setMetroInicio(int metroInicio) { this->metroInicio = metroInicio;	}
	int getMetroInicio() { return this->metroInicio; }

protected:
	int longitud;
	TipoTramo tipo;
	int id;
	int metroInicio;
};
#endif