#pragma once
#ifndef MAPA_H
#define MAPA_H

#include "ObjetoFijo.h"
#include "Tramo.h"
#include <vector>

const std::string ARCHIVO_XML_MAPA = "mapa.xml";

class Mapa {
public:
	Mapa();
	std::vector<Tramo*> getTramosDelMapa();
	std::vector<ObjetoFijo*> getObjetosDelMapa();
	void setObjetosDelMapa(std::vector<ObjetoFijo*> objetosDelMapa) { this->objetosDelMapa = objetosDelMapa; }
private:
	void cargarMapaDesdeXML();
	std::vector<Tramo*> tramosDelMapa;
	std::vector<ObjetoFijo*> objetosDelMapa;
};

#endif