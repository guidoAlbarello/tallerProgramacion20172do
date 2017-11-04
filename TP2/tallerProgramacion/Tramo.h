#pragma once
#ifndef TRAMO_H
#define TRAMO_H
#include "Tramo.h"
#include "ObjetoFijo.h"

enum TipoTramo { Recta = 0, Curva = 1 };
enum SentidoCurva { SCIzquierda = 0, SCDerecha = 1, NINGUNO = 2 };

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
	SentidoCurva getSentidoCurva() { return this->sentido; }
	void setSentidoCurva(SentidoCurva sentidoCurva) { this->sentido = sentidoCurva; }
protected:
	int longitud;
	TipoTramo tipo;
	int id;
	int metroInicio;
	SentidoCurva sentido;
};
#endif