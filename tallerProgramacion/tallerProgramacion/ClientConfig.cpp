#include "ClientConfig.h"
#include "ParserXml.h"
#include "../lib/rapidxml-1.13/rapidxml.hpp"
#include "../lib/rapidxml-1.13/rapidxml_print.hpp"
#include <fstream>
#include "Logger.h"

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

		rapidxml::xml_node<>* nodoConexion = nodoCliente->first_node("conexion");

		std::string numeroPuerto = nodoConexion->first_node("puerto")->value();

		this->puerto = numeroPuerto;

		rapidxml::xml_node<>* nodoDireccionIP = nodoConexion->first_node("IP");
		rapidxml::xml_node<>* nodoTestfilePath = nodoCliente->first_node("testfile")->first_node("path");

		this->IP = nodoDireccionIP->value();
		this->path = nodoTestfilePath->value();

		Logger::getInstance()->log(LogMode::Debug, "[" + DEBUG_CLIENT_TAG + "] " + this->nombreConfiguracionPredeterminada + " se parseo exitosamente.");
	}
	catch (std::exception& e) {
		Logger::getInstance()->log(LogMode::Error, "[" + DEBUG_CLIENT_TAG + "] Hubo un error al parsear el archivo de configuracion del cliente.");
		cout << e.what();
	}
}