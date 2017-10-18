#pragma once
#ifndef TRAMO_H
#define TRAMO_H

enum TipoTramo { Recta = 0, Curva = 1 };

class Tramo {
public:
	int getLongitud();
	void setLongitud(int longitud) {
		this->longitud = longitud;
	}
	TipoTramo getTipoTramo();
protected:
	int longitud;
	TipoTramo tipo;
};
#endif