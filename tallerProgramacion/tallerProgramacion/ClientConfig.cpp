#include "ClientConfig.h"
#include "ParserXml.h"
#include "../lib/rapidxml-1.13/rapidxml.hpp"
#include "../lib/rapidxml-1.13/rapidxml_print.hpp"
#include <fstream>

ClientConfig::ClientConfig() {
	this->IP = DEFAULT_IP;
	this->puerto = DEFAULT_PUERTO_CLIENTE;
	this->path = DEFAULT_TESTFILE;
}

ClientConfig::ClientConfig(std::string unaIP, std::string unPuerto, std::string path) {
	this->IP = unaIP;
	this->puerto = unPuerto;
	this->path = path;
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

void ClientConfig::crearArchivoConfiguracion(std::string nombre) {
	//Generando un nuevo archivo de configuracion
	rapidxml::xml_document<> archivoXML;

	rapidxml::xml_node<>* nodoCliente = archivoXML.allocate_node(rapidxml::node_element, "cliente");
	rapidxml::xml_node<>* nodoConexion = archivoXML.allocate_node(rapidxml::node_element, "conexion");
	rapidxml::xml_node<>* nodoIP = archivoXML.allocate_node(rapidxml::node_element, "IP", DEFAULT_IP.c_str());
	rapidxml::xml_node<>* nodoPuerto = archivoXML.allocate_node(rapidxml::node_element, "puerto", DEFAULT_PUERTO_CLIENTE.c_str());

	nodoConexion->append_node(nodoIP);
	nodoConexion->append_node(nodoPuerto);

	rapidxml::xml_node<>* nodoPath = archivoXML.allocate_node(rapidxml::node_element, "path");
	rapidxml::xml_node<>* nodoTestfile = archivoXML.allocate_node(rapidxml::node_element, "testfile", DEFAULT_TESTFILE.c_str());

	nodoPath->append_node(nodoTestfile);

	nodoCliente->append_node(nodoConexion);
	nodoCliente->append_node(nodoPath);

	archivoXML.append_node(nodoCliente);

	std::ofstream archivo;
	archivo.open(nombre);
	archivo << archivoXML;
}