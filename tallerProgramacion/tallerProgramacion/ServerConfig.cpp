#include "ServerConfig.h"
#include <vector>
#include "Usuario.h"
#include "ParserXml.h"
#include "../lib/rapidxml-1.13/rapidxml.hpp"
#include "../lib/rapidxml-1.13/rapidxml_print.hpp"
#include <fstream>

ServerConfig::ServerConfig() {
	this->maxClientes = atoi(DEFAULT_MAXCLIENTES.c_str());
	this->puerto = DEFAULT_PUERTO_SERVIDOR;
}

void ServerConfig::crearArchivoConfiguracion(std::string nombre) {
	//Generando un nuevo archivo de configuracion
	rapidxml::xml_document<> archivoXML;
	rapidxml::xml_node<>* nodoServidor = archivoXML.allocate_node(rapidxml::node_element, "servidor");
	rapidxml::xml_node<>* nodoMaxClientes = archivoXML.allocate_node(rapidxml::node_element, "cantidadMaximaClientes", DEFAULT_MAXCLIENTES.c_str());
	rapidxml::xml_node<>* nodoPuerto = archivoXML.allocate_node(rapidxml::node_element, "puerto", DEFAULT_PUERTO_SERVIDOR.c_str());

	nodoServidor->append_node(nodoMaxClientes);
	nodoServidor->append_node(nodoPuerto);
	// Creo un usuario predeterminado
	rapidxml::xml_node<>* nodoUsuarios = archivoXML.allocate_node(rapidxml::node_element, "usuarios", "");
	rapidxml::xml_node<>* nodoNuevoUsuario = archivoXML.allocate_node(rapidxml::node_element, "usuario", "");
	rapidxml::xml_node<>* nodoNombreAdmin = archivoXML.allocate_node(rapidxml::node_element, "nombre", "Admin");
	rapidxml::xml_node<>* nodoPasswordAdmin = archivoXML.allocate_node(rapidxml::node_element, "password", "admin");

	nodoNuevoUsuario->append_node(nodoNombreAdmin);
	nodoNuevoUsuario->append_node(nodoPasswordAdmin);
	nodoUsuarios->append_node(nodoNuevoUsuario);

	archivoXML.append_node(nodoServidor);
	archivoXML.append_node(nodoUsuarios);

	std::ofstream archivo;
	archivo.open(nombre);
	archivo << archivoXML;
}

int ServerConfig::getMaxClientes() {
	return this->maxClientes;
}

void ServerConfig::setMaxClientes(int maxClientes) {
	this->maxClientes = maxClientes;
}

std::string ServerConfig::getPuerto() {
	return this->puerto;
}
void ServerConfig::setPuerto(std::string puerto) {
	this->puerto = puerto;
}

std::vector<Usuario> ServerConfig::getUsuarios() {
	return this->usuarios;
}

void ServerConfig::setUsuarios(std::vector<Usuario> usuarios) {
	this->usuarios = usuarios;
}
