#include "Config.h"
#include "../lib/rapidxml-1.13/rapidxml_print.hpp"
#include <fstream>

Config::Config() {
}

void Config::leerConfiguracion() {
	if (this->existeArchivo(this->nombreConfiguracionPredeterminada)) {
		this->parsearArchivoXML(this->nombreConfiguracionPredeterminada);
	}
	else {
		this->crearConfiguracionPredeterminada();
	}
}

bool Config::existeArchivo(const std::string& nombreDeArchivo) {
	std::ifstream archivo(nombreDeArchivo.c_str());
	return (bool)archivo;
}

void Config::grabarDocumentoXML(std::string nombre, rapidxml::xml_document<>* documentoXML) {
	if (!existeArchivo(nombre)) {
		std::ofstream archivo;
		archivo.open(nombre);
		archivo << *(documentoXML);
		archivo.close();
	}
}