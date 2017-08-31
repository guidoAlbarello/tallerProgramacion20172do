#include "ParserXml.h"
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>     
#include "../lib/rapidxml-1.13/rapidxml.hpp"
#include "Usuario.h"

using namespace std;

ParserXml::ParserXml() {

}

ServerConfig* ParserXml::openServerConfigFile(std::string path) {
	try {
		// TODO: pasar los prints al logger
		cout << "Leyendo xml..." << endl;
		rapidxml:: xml_document<> documento;
		ifstream archivo(path);
		vector<char> buffer((istreambuf_iterator<char>(archivo)), istreambuf_iterator<char>());
		buffer.push_back('\0');
		documento.parse<0>(&buffer[0]); // <0> == sin flags de parseo

		rapidxml::xml_node<>* nodoServidor = documento.first_node("servidor");

		rapidxml::xml_node<>* nodoMaxClientes = nodoServidor->first_node("cantidadMaximaClientes");
		std::string maxClientes = nodoMaxClientes->value();
		int intMaxClientes = atoi(maxClientes.c_str());

		rapidxml::xml_node<>* nodoPuerto = nodoMaxClientes->next_sibling();
		std::string numeroPuerto = nodoPuerto->value();
		int intMaxPuertos = atoi(numeroPuerto.c_str());

		rapidxml::xml_node<>* nodoUsuarios = documento.first_node("usuarios");
		std::vector<Usuario> usuarios;
		for (rapidxml::xml_node<>* unNodoUsuario = nodoUsuarios->first_node("usuario"); unNodoUsuario; unNodoUsuario = unNodoUsuario->next_sibling()) {
			rapidxml::xml_node<>* nodoNombre = unNodoUsuario->first_node("nombre");
			rapidxml::xml_node<>* nodoPassword = unNodoUsuario->first_node("password");
			Usuario user(nodoNombre->value(), nodoPassword->value());
			usuarios.push_back(user);
		}

		cout << "Fin lectura xml" << endl;

		ServerConfig serverConfig(intMaxClientes, intMaxPuertos, usuarios);
		return &serverConfig;
	} catch (std::exception& e) {
		cout << "Ocurrio un error al parsear el archivo de configuracon del servidor" << endl;
		cout << e.what();
	}
}

ClientConfig* ParserXml::openClientConfigFile(std::string path) {
	cout << "Leyendo xml..." << endl;
	rapidxml::xml_document<> documento;
	ifstream archivo(path);
	vector<char> buffer((istreambuf_iterator<char>(archivo)), istreambuf_iterator<char>());
	buffer.push_back('\0');
	documento.parse<0>(&buffer[0]); // <0> == sin flags de parseo

	rapidxml::xml_node<>* nodoCliente = documento.first_node("cliente");

	rapidxml::xml_node<>* nodoConexion = nodoCliente->first_node("conexion");

	std::string numeroPuerto = nodoConexion->first_node("puerto")->value();
	int intPuerto = atoi(numeroPuerto.c_str());

	rapidxml::xml_node<>* nodoDireccionIP = nodoConexion->first_node("IP");
	rapidxml::xml_node<>* nodoTestfilePath = nodoCliente->first_node("testfile")->first_node("path");

	
	cout << "Fin lectura xml" << endl;

	ClientConfig clientConfig(nodoDireccionIP->value(), intPuerto, nodoTestfilePath->value());
	return &clientConfig;
}

