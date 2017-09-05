#include "ServerConfig.h"
#include <vector>
#include "Usuario.h"
#include "ParserXml.h"
#include "../lib/rapidxml-1.13/rapidxml.hpp"
#include "../lib/rapidxml-1.13/rapidxml_print.hpp"
#include <fstream>
#include "Logger.h"

ServerConfig::ServerConfig() {
	this->nombreConfiguracionPredeterminada = DEFAULT_SERVER_CONFIG;

	this->maxClientes = atoi(DEFAULT_MAXCLIENTES.c_str());
	this->puerto = DEFAULT_PUERTO_SERVIDOR;
}

void ServerConfig::crearConfiguracionPredeterminada() {
	Logger::getInstance()->log(LogMode::Debug, "[" + DEBUG_SERVER_TAG + "] Creando configuracion de servidor predeterminada: " + this->nombreConfiguracionPredeterminada);
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
	
	Usuario usuario("Admin", "admin");
	this->usuarios.push_back(&usuario);

	nodoNuevoUsuario->append_node(nodoNombreAdmin);
	nodoNuevoUsuario->append_node(nodoPasswordAdmin);
	nodoUsuarios->append_node(nodoNuevoUsuario);

	archivoXML.append_node(nodoServidor);
	archivoXML.append_node(nodoUsuarios);

	this->grabarDocumentoXML(this->nombreConfiguracionPredeterminada, &archivoXML);
	Logger::getInstance()->log(LogMode::Debug, "[" + DEBUG_SERVER_TAG + "] " + this->nombreConfiguracionPredeterminada + " se creo exitosamente.");
	Logger::getInstance()->log(LogMode::Debug, "[" + DEBUG_SERVER_TAG + "] " + "Configuracion del servidor (PREDETERMINADA): PUERTO: " + DEFAULT_PUERTO_SERVIDOR.c_str() + ", MAXIMOS CLIENTES: " + DEFAULT_MAXCLIENTES.c_str() + ".");
}

void ServerConfig::parsearArchivoXML(std::string nombre) {
	try {
		Logger::getInstance()->log(LogMode::Debug, "[" + DEBUG_SERVER_TAG + "] Parseando configuracion del servidor: " + this->nombreConfiguracionPredeterminada);
		rapidxml::xml_document<> documento;
		ifstream archivo(nombre);
		vector<char> buffer((istreambuf_iterator<char>(archivo)), istreambuf_iterator<char>());
		buffer.push_back('\0');
		documento.parse<0>(&buffer[0]); // <0> == sin flags de parseo

		rapidxml::xml_node<>* nodoServidor = documento.first_node("servidor");

		rapidxml::xml_node<>* nodoMaxClientes = nodoServidor->first_node("cantidadMaximaClientes");
		std::string maxClientes = nodoMaxClientes->value();
		this->maxClientes = atoi(maxClientes.c_str());

		rapidxml::xml_node<>* nodoPuerto = nodoMaxClientes->next_sibling();
		std::string numeroPuerto = nodoPuerto->value();
		this->puerto = numeroPuerto;

		rapidxml::xml_node<>* nodoUsuarios = documento.first_node("usuarios");
		std::vector<Usuario *> usuarios;
		for (rapidxml::xml_node<>* unNodoUsuario = nodoUsuarios->first_node("usuario"); unNodoUsuario; unNodoUsuario = unNodoUsuario->next_sibling()) {
			rapidxml::xml_node<>* nodoNombre = unNodoUsuario->first_node("nombre");
			rapidxml::xml_node<>* nodoPassword = unNodoUsuario->first_node("password");
			Usuario user(nodoNombre->value(), nodoPassword->value());
			usuarios.push_back(&user);
		}
		this->usuarios = usuarios;

		Logger::getInstance()->log(LogMode::Debug, "[" + DEBUG_SERVER_TAG + "] " + this->nombreConfiguracionPredeterminada + " se parseo exitosamente.");
		Logger::getInstance()->log(LogMode::Debug, "[" + DEBUG_SERVER_TAG + "] " + "Configuracion del servidor: PUERTO: " + this->puerto + ", MAXIMOS CLIENTES: " + maxClientes.c_str() + ".");
	}
	catch (std::exception& e) {
		Logger::getInstance()->log(LogMode::Error, "[" + DEBUG_SERVER_TAG + "] Hubo un error al parsear el archivo de configuracion del servidor.");
		cout << e.what();
	}
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

std::vector<Usuario *> ServerConfig::getUsuarios() {
	return this->usuarios;
}

void ServerConfig::setUsuarios(std::vector<Usuario *> usuarios) {
	this->usuarios = usuarios;
}