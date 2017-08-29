#include "ParserXml.h"
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>     
#include "../lib/rapidxml-1.13/rapidxml.hpp"
#include "Usuario.h"
#include "ServerConfig.h"

using namespace rapidxml;
using namespace std;


ServerConfig* ParserXml::openServerConfigFile(std::string path) {
	// TODO: pasar los prints al logger
	cout << "Leyendo xml..." << endl;
	xml_document<> documento;
	ifstream archivo(path);
	vector<char> buffer((istreambuf_iterator<char>(archivo)), istreambuf_iterator<char>());
	buffer.push_back('\0');
	documento.parse<0>(&buffer[0]); // <0> == sin flags de parseo

	xml_node<>* nodoServidor;
	nodoServidor = documento.first_node("servidor");

	xml_node<>* nodoMaxClientes = nodoServidor->first_node("cantidadMaximaClientes");
	std::string maxClientes = nodoMaxClientes->value();
	int intMaxClientes = atoi(maxClientes.c_str());

	xml_node<>* nodoPuerto = nodoMaxClientes->next_sibling();
	std::string numeroPuerto = nodoPuerto->value();
	int intMaxPuertos = atoi(numeroPuerto.c_str());

	xml_node<>* nodoUsuarios;
	nodoUsuarios = documento.first_node("usuarios");
	std::vector<Usuario> usuarios;
	for (xml_node<>* unNodoUsuario = nodoUsuarios->first_node("usuario"); unNodoUsuario; unNodoUsuario = unNodoUsuario->next_sibling()) {
		xml_node<>* nodoNombre = unNodoUsuario->first_node("nombre");
		xml_node<>* nodoPassword = unNodoUsuario->first_node("password");
		Usuario user(nodoNombre->value(), nodoPassword->value());
		usuarios.push_back(user);
	}
	
	cout << "Fin lectura xml" << endl;
	ServerConfig serverConfig(intMaxClientes, intMaxPuertos, usuarios);
	return &serverConfig;
}

void ParserXml::openClientConfigFile(std::string path) {

}
