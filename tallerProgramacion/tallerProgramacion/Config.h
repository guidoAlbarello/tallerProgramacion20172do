#pragma once
#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include "../lib/rapidxml-1.13/rapidxml.hpp"

using namespace std;

class Config {
public:
	Config();
	void leerConfiguracion();
protected:
	std::string nombreConfiguracionPredeterminada;
	bool existeArchivo(const std::string& nombreDeArchivo);
	virtual void crearConfiguracionPredeterminada() = 0;
	virtual void parsearArchivoXML(std::string nombre) = 0;
	void grabarDocumentoXML(std::string nombre, rapidxml::xml_document<>* documentoXML);
};

#endif
