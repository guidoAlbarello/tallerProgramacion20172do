#include "Mapa.h"
#include <iostream>
#include <fstream>
#include "../lib/rapidxml-1.13/rapidxml.hpp"
#include "../lib/rapidxml-1.13/rapidxml_print.hpp"
#include "Logger.h"
#include "Tramo.h"
#include "TramoRecto.h"
#include "TramoCurvo.h"
#include "Arbol.h"
#include "Cartel.h"

using namespace std;

namespace funcionesConversoras {
	SentidoCurva obtenerSentidoCurva(std::string sentido) {
		return (sentido == "derecha" ? SentidoCurva::SCDerecha : SentidoCurva::SCIzquierda);
	}

	TipoTramo obtenerTipoDeTramo(std::string tipo) {
		return (tipo == "recta" ? TipoTramo::Recta : TipoTramo::Curva);
	}

	Posicion obtenerPosicion(std::string posicion) {
		return (posicion == "derecha" ? Posicion::PDerecha : Posicion::PIzquierda);
	}

	MaximaVelocidad obtenerMaximaVelocidad(std::string valor) {
		if (valor == "120")
			return MaximaVelocidad::V120;
		else if (valor == "80")
			return MaximaVelocidad::V80;

		return MaximaVelocidad::V60;
	}

}

using namespace funcionesConversoras;

Mapa::Mapa(int nivel) {
	this->longitudTotal = 0;
	this->cargarMapaDesdeXML(nivel);
}

std::vector<ObjetoFijo*> Mapa::getObjetosDelMapa() {
	return this->objetosDelMapa;
}

std::vector<Tramo*> Mapa::getTramosDelMapa() {
	return this->tramosDelMapa;
}

void Mapa::cargarMapaDesdeXML(int nivel) {
	try {
		Logger::getInstance()->log(LogMode::Debug, "[MAPA] Parseando el mapa desde el archivo: " + ARCHIVO_MAPA + to_string(nivel) + EXTENSION_MAPA);

		rapidxml::xml_document<> documento;
		ifstream archivo(ARCHIVO_MAPA + to_string(nivel) + EXTENSION_MAPA);
		vector<char> buffer((istreambuf_iterator<char>(archivo)), istreambuf_iterator<char>());
		buffer.push_back('\0');
		documento.parse<0>(&buffer[0]); // <0> == sin flags de parseo

		rapidxml::xml_node<>* nodoMapa = documento.first_node("mapa");

		if (nodoMapa == NULL) {
			//Si no hay nodo mapa (root) no se puede seguir leyendo
			Logger::getInstance()->log(LogMode::Error, "[MAPA] No se encontro el nodo de 'mapa' en " + ARCHIVO_MAPA + to_string(nivel) + EXTENSION_MAPA + ". No se cargara el mapa.");
			return;
		}

		//Itero entre los tramos y analizo cada uno de ellos
		int contadorTramos = 0;
		int metrosAcumulados = 0;
		for (rapidxml::xml_node<>* unNodoTramo = nodoMapa->first_node("tramo"); unNodoTramo; unNodoTramo = unNodoTramo->next_sibling("tramo")) {
			rapidxml::xml_attribute<>* attTipoTramo = unNodoTramo->first_attribute("tipo");
			rapidxml::xml_attribute<>* attLongitud = unNodoTramo->first_attribute("longitud");
			rapidxml::xml_attribute<>* attSentido;
			if (attTipoTramo == NULL || attLongitud == NULL) {
				Logger::getInstance()->log(LogMode::Debug, "[MAPA] Se encontro un tramo sin atributo 'tipo' o 'longitud'. Se procede a ignorar el tramo.");
				continue;
			}
			if (attLongitud != NULL) {
				this->longitudTotal = this->longitudTotal + stoi(attLongitud->value());
			}
			TipoTramo tipoTramo = obtenerTipoDeTramo(attTipoTramo->value());

			if (tipoTramo == TipoTramo::Recta) {
				TramoRecto* tramoRecto = new TramoRecto(stoi(attLongitud->value()));
				tramoRecto->setId(contadorTramos);
				tramoRecto->setMetroInicio(metrosAcumulados);
				tramoRecto->setSentidoCurva(SentidoCurva::NINGUNO);
				metrosAcumulados += stoi(attLongitud->value());
				this->tramosDelMapa.push_back(tramoRecto);
			}
			else {
				attSentido = unNodoTramo->first_attribute("sentido");
				if (attSentido == NULL) {
					Logger::getInstance()->log(LogMode::Debug, "[MAPA] Se encontro un tramo de tipo 'curva' sin el atributo 'sentido'. Se procede a ignorar el tramo.");
					continue;
				}
				TramoCurvo* tramoCurvo = new TramoCurvo(stoi(attLongitud->value()), obtenerSentidoCurva(attSentido->value()));
				tramoCurvo->setId(contadorTramos);
				tramoCurvo->setMetroInicio(metrosAcumulados);
				metrosAcumulados += stoi(attLongitud->value());
				this->tramosDelMapa.push_back(tramoCurvo);
			}

			// Objetos del tramo
			for (rapidxml::xml_node<>* unNodo = unNodoTramo->first_node(); unNodo; unNodo = unNodo->next_sibling()) {
				std::string nombreNodo = unNodo->name();
				rapidxml::xml_attribute<>* attPosicion = unNodo->first_attribute("posicion");
				rapidxml::xml_attribute<>* attUbicacion = unNodo->first_attribute("ubicacion");

				//Ignoro el nodo si no encuentro algun atributo necesario
				if (attPosicion == NULL || attUbicacion == NULL) {
					Logger::getInstance()->log(LogMode::Debug, "[MAPA] Se encontro un nodo sin atributo 'ubicacion' o 'posicion'. Se procede a ignorar el nodo.");
					continue;
				}

				if (nombreNodo == "arbol") {
					ObjetoFijo* objeto = new Arbol(stoi(attUbicacion->value()), obtenerPosicion(attPosicion->value()));
					objeto->setTramo(contadorTramos);
					objeto->setTipoObjeto(TipoObjeto::ARBOL);
					this->objetosDelMapa.push_back(objeto);
				}
				else if (nombreNodo == "cartel") {
					rapidxml::xml_attribute<>* attValor = unNodo->first_attribute("valor");
					if (attValor == NULL) {
						Logger::getInstance()->log(LogMode::Debug, "[MAPA] Se encontro un cartel sin el atributo 'valor'. Se procede a ignorar el cartel.");
						continue;
					}
					ObjetoFijo* objeto = new Cartel(stoi(attUbicacion->value()), obtenerPosicion(attPosicion->value()), obtenerMaximaVelocidad(attValor->value()));
					objeto->setTramo(contadorTramos);
					objeto->setTipoObjeto(TipoObjeto::CARTEL);
					objeto->setValor(attValor->value());
					this->objetosDelMapa.push_back(objeto);
				}
			}

			contadorTramos++;
		}

		Logger::getInstance()->log(LogMode::Actividad, "[MAPA] Finalizo la lectura del mapa");
	}
	catch (std::exception& e) {
		Logger::getInstance()->log(LogMode::Error, "[MAPA] Hubo un error al parsear el archivo del mapa.");
		cout << e.what();
	}
}