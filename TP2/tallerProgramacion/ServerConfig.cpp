#include "ServerConfig.h"
#include <vector>
#include "Usuario.h"
#include "../lib/rapidxml-1.13/rapidxml.hpp"
#include "../lib/rapidxml-1.13/rapidxml_print.hpp"
#include <fstream>
#include "Logger.h"

ServerConfig::ServerConfig() {
	this->nombreConfiguracionPredeterminada = DEFAULT_SERVER_CONFIG;
	this->maxClientes = atoi(DEFAULT_MAXCLIENTES.c_str());
	this->puerto = DEFAULT_PUERTO_SERVIDOR;
}

ServerConfig::~ServerConfig() {
	for (std::vector<Usuario*>::iterator it = this->usuarios.begin(); it != this->usuarios.end(); ++it) {
		delete (*it);
	}
	this->usuarios.clear();
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
	
	this->cargarUsuariosPredeterminados();

	nodoNuevoUsuario->append_node(nodoNombreAdmin);
	nodoNuevoUsuario->append_node(nodoPasswordAdmin);
	nodoUsuarios->append_node(nodoNuevoUsuario);

	archivoXML.append_node(nodoServidor);
	archivoXML.append_node(nodoUsuarios);

	this->grabarDocumentoXML(this->nombreConfiguracionPredeterminada, &archivoXML);
	Logger::getInstance()->log(LogMode::Debug, "[" + DEBUG_SERVER_TAG + "] " + this->nombreConfiguracionPredeterminada + " se creo exitosamente.");
	Logger::getInstance()->log(LogMode::Debug, "[" + DEBUG_SERVER_TAG + "] " + "Configuracion del servidor (PREDETERMINADA): PUERTO: " + DEFAULT_PUERTO_SERVIDOR.c_str() + ", MAXIMOS CLIENTES: " + DEFAULT_MAXCLIENTES.c_str() + ".");
}

void ServerConfig::cargarUsuariosPredeterminados() {
	this->usuarios.push_back(new Usuario("Admin", "admin"));
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
		if (nodoServidor != NULL) {
			rapidxml::xml_node<>* nodoMaxClientes = nodoServidor->first_node("cantidadMaximaClientes");
			if (nodoMaxClientes != NULL) {
				std::string maxClientes = nodoMaxClientes->value();
				this->maxClientes = atoi(maxClientes.c_str());
			}
			else {
				Logger::getInstance()->log(LogMode::Error, "[" + DEBUG_SERVER_TAG + "] " + "No se encontro el nodo 'cantidadMaximaClientes' en la configuracion. A continuacion, se cargara la cantidad de maximos clientes predeterminada.");
			}

			rapidxml::xml_node<>* nodoPuerto = nodoServidor->first_node("puerto");
			if (nodoPuerto != NULL) {
				std::string numeroPuerto = nodoPuerto->value();
				this->puerto = numeroPuerto;
			}
			else {
				Logger::getInstance()->log(LogMode::Error, "[" + DEBUG_SERVER_TAG + "] " + "No se pudo obtener el numero de puerto desde el archivo de configuracion XML.");
			}
		}
		else {
			Logger::getInstance()->log(LogMode::Error, "[" + DEBUG_SERVER_TAG + "] " + "No se encontro el nodo 'servidor' en la configuracion. A continuacion, se cargara la cantidad de maximos clientes y el numero de puerto predeterminados.");
		}

		rapidxml::xml_node<>* nodoUsuarios = documento.first_node("usuarios");
		if (nodoUsuarios != NULL) {
			std::vector<Usuario*> usuarios;
			for (rapidxml::xml_node<>* unNodoUsuario = nodoUsuarios->first_node("usuario"); unNodoUsuario; unNodoUsuario = unNodoUsuario->next_sibling("usuario")) {
				rapidxml::xml_node<>* nodoNombre = unNodoUsuario->first_node("nombre");
				rapidxml::xml_node<>* nodoPassword = unNodoUsuario->first_node("password");
				if (nodoNombre != NULL && nodoPassword != NULL) {
					usuarios.push_back(new Usuario(nodoNombre->value(), nodoPassword->value()));
					Logger::getInstance()->log(LogMode::Actividad, "[" + DEBUG_SERVER_TAG + "] " + "Se leyo el usuario USUARIO: '" + nodoNombre->value() + "'-CONTRASENIA='" + nodoPassword->value() + "' desde el archivo de configuracion XML.");
				}
				else {
					Logger::getInstance()->log(LogMode::Error, "[" + DEBUG_SERVER_TAG + "] " + "No se pudo leer un usuario desde el archivo de configuracion porque el nodo 'nombre' o el nodo 'password' no existe.");
				}
			}
			this->usuarios = usuarios;
		}
		else {
			Logger::getInstance()->log(LogMode::Error, "[" + DEBUG_SERVER_TAG + "] " + "No se encontro el nodo 'usuarios' en el archivo de configuracion XML. Se procede a cargar los usuarios predeterminados.");
			//Cargo usuarios predeterminados
			this->cargarUsuariosPredeterminados();
		}

		Logger::getInstance()->log(LogMode::Debug, "[" + DEBUG_SERVER_TAG + "] " + this->nombreConfiguracionPredeterminada + " se parseo exitosamente.");
		Logger::getInstance()->log(LogMode::Debug, "[" + DEBUG_SERVER_TAG + "] " + "Configuracion del servidor: PUERTO: " + this->puerto + ", MAXIMOS CLIENTES: " + to_string(this->maxClientes) + ".");
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

std::vector<Usuario*> ServerConfig::getUsuarios() {
	return this->usuarios;
}

void ServerConfig::setUsuarios(std::vector<Usuario*> usuarios) {
	this->usuarios = usuarios;
}