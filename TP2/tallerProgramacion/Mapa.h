#pragma once
#ifndef MAPA_H
#define MAPA_H

#include "ObjetoFijo.h"
#include "Tramo.h"
#include <vector>

const std::string ARCHIVO_MAPA = "mapa";
const std::string EXTENSION_MAPA = ".xml";

class Mapa {
public:
	Mapa(int nivel);
	std::vector<Tramo*> getTramosDelMapa();
	std::vector<ObjetoFijo*> getObjetosDelMapa();
	void setObjetosDelMapa(std::vector<ObjetoFijo*> objetosDelMapa) { this->objetosDelMapa = objetosDelMapa; }
	int getLongitudTotal() { return this->longitudTotal; }
private:
	void cargarMapaDesdeXML(int nivel);
	std::vector<Tramo*> tramosDelMapa;
	std::vector<ObjetoFijo*> objetosDelMapa;
	int longitudTotal;
};

#endif