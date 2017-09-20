#include "ClientConfig.h"
#include "../lib/rapidxml-1.13/rapidxml.hpp"
#include "../lib/rapidxml-1.13/rapidxml_print.hpp"
#include <fstream>
#include <vector>
#include "Logger.h"
#include <iostream>

ClientConfig::ClientConfig() {
	this->nombreConfiguracionPredeterminada = DEFAULT_CLIENT_CONFIG;

	this->IP = DEFAULT_IP;
	this->puerto = DEFAULT_PUERTO_CLIENTE;
	this->path = DEFAULT_PATH;
}

void ClientConfig::setPuerto(std::string unPuerto) {
	this->puerto = unPuerto;
}

void ClientConfig::setIP(std::string unaIP) {
	this->IP = unaIP;
}

void ClientConfig::setPath(std::string path) {
	this->path = path;
}

std::string ClientConfig::getPuerto() {
	return this->puerto;
}

std::string ClientConfig::getIP() {
	return this->IP;
}

std::string ClientConfig::getPath() {
	return this->path;
}

void ClientConfig::crearConfiguracionPredeterminada() {
	Logger::getInstance()->log(LogMode::Debug, "[" + DEBUG_CLIENT_TAG + "] Creando configuracion de cliente predeterminada: " + this->nombreConfiguracionPredeterminada);
	//Generando un nuevo archivo de configuracion
	rapidxml::xml_document<> archivoXML;

	rapidxml::xml_node<>* nodoCliente = archivoXML.allocate_node(rapidxml::node_element, "cliente");
	rapidxml::xml_node<>* nodoConexion = archivoXML.allocate_node(rapidxml::node_element, "conexion");
	rapidxml::xml_node<>* nodoIP = archivoXML.allocate_node(rapidxml::node_element, "IP", DEFAULT_IP.c_str());
	rapidxml::xml_node<>* nodoPuerto = archivoXML.allocate_node(rapidxml::node_element, "puerto", DEFAULT_PUERTO_CLIENTE.c_str());
	
	nodoConexion->append_node(nodoIP);
	nodoConexion->append_node(nodoPuerto);

	rapidxml::xml_node<>* nodoTestfile = archivoXML.allocate_node(rapidxml::node_element, "testfile");
	rapidxml::xml_node<>* nodoPath = archivoXML.allocate_node(rapidxml::node_element, "path", DEFAULT_PATH.c_str());

	nodoTestfile->append_node(nodoPath);

	nodoCliente->append_node(nodoConexion);
	nodoCliente->append_node(nodoTestfile);

	archivoXML.append_node(nodoCliente);

	this->grabarDocumentoXML(this->nombreConfiguracionPredeterminada, &archivoXML);
	Logger::getInstance()->log(LogMode::Debug, "[" + DEBUG_CLIENT_TAG + "] " + this->nombreConfiguracionPredeterminada + " se creo exitosamente.");
	Logger::getInstance()->log(LogMode::Debug, "[" + DEBUG_CLIENT_TAG + "] " + "Configuracion del cliente (PREDETERMINADA): IP:PUERTO: " + DEFAULT_IP + ":" + DEFAULT_PUERTO_CLIENTE + ", PATH: " + DEFAULT_PATH + ".");
}

void ClientConfig::parsearArchivoXML(std::string nombre) {
	try {
		Logger::getInstance()->log(LogMode::Debug, "[" + DEBUG_CLIENT_TAG + "] Parseando configuracion del cliente: " + this->nombreConfiguracionPredeterminada);

		rapidxml::xml_document<> documento;
		ifstream archivo(nombre);
		vector<char> buffer((istreambuf_iterator<char>(archivo)), istreambuf_iterator<char>());
		buffer.push_back('\0');
		documento.parse<0>(&buffer[0]); // <0> == sin flags de parseo

		rapidxml::xml_node<>* nodoCliente = documento.first_node("cliente");

		if (nodoCliente == NULL) {
			//Si no hay nodo cliente (root) no se puede seguir leyendo
			return;
		}
		else {
			Logger::getInstance()->log(LogMode::Error, "[" + DEBUG_CLIENT_TAG + "] " + "No se encontro el nodo de 'cliente' en la configuracion. A continuacion, se cargara la configuracion predeterminada.");
		}

		rapidxml::xml_node<>* nodoConexion = nodoCliente->first_node("conexion");

		if (nodoConexion != NULL) {
			rapidxml::xml_node<>* nodoDireccionIP = nodoConexion->first_node("IP");
			if (nodoDireccionIP != NULL) {
				this->IP = nodoDireccionIP->value();
			}
			else {
				Logger::getInstance()->log(LogMode::Error, "[" + DEBUG_CLIENT_TAG + "] " + "No se pudo obtener la direccion IP desde el archivo de configuracion XML.");
			}

			rapidxml::xml_node<>* nodoPuerto = nodoConexion->first_node("puerto");
			if (nodoPuerto != NULL) {
				this->puerto = nodoPuerto->value();
			}
			else {
				Logger::getInstance()->log(LogMode::Error, "[" + DEBUG_CLIENT_TAG + "] " + "No se pudo obtener el numero de puerto desde el archivo de configuracion XML.");
			}
		}
		else {
			Logger::getInstance()->log(LogMode::Error, "[" + DEBUG_CLIENT_TAG + "] " + "No se pudo encontrar el nodo conexion dentro del archivo de configuracion de XML.");
		}

		rapidxml::xml_node<>* nodoTestfile = nodoCliente->first_node("testfile");
		if (nodoTestfile != NULL) {
			rapidxml::xml_node<>* nodoPath = nodoTestfile->first_node("path");
			if (nodoPath != NULL) {
				this->path = nodoPath->value();
			}
			else {
				Logger::getInstance()->log(LogMode::Error, "[" + DEBUG_CLIENT_TAG + "] " + "No se pudo obtener la direccion del archivo de estres desde el archivo de configuracion XML.");
			}
		}
		else {
			Logger::getInstance()->log(LogMode::Error, "[" + DEBUG_CLIENT_TAG + "] " + "No se encontro el nodo 'testfile' en la configuracion. A continuacion, se cargara el path predeterminado.");
		}

		Logger::getInstance()->log(LogMode::Debug, "[" + DEBUG_CLIENT_TAG + "] " + this->nombreConfiguracionPredeterminada + " se parseo exitosamente.");
		Logger::getInstance()->log(LogMode::Debug, "[" + DEBUG_CLIENT_TAG + "] " + "Configuracion del cliente: IP:PUERTO: " + this->IP + this->puerto + ", PATH: " + this->path + ".");
	}
	catch (std::exception& e) {
		Logger::getInstance()->log(LogMode::Error, "[" + DEBUG_CLIENT_TAG + "] Hubo un error al parsear el archivo de configuracion del cliente.");
		cout << e.what();
	}
}