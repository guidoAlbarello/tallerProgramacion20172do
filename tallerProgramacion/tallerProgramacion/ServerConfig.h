#pragma once
#ifndef SERVERCONFIG_H
#define SERVERCONFIG_H

#include "Usuario.h"
#include <vector>

using namespace std;

const std::string DEFAULT_MAXCLIENTES = "3";
const std::string DEFAULT_PUERTO_SERVIDOR = "12345";

class ServerConfig {
	int maxClientes;
	std::string puerto;
	std::vector<Usuario *> usuarios;
public:
	ServerConfig();
	//ServerConfig(int maxClientes, int puerto, std::vector<Usuario> usuarios);
	int getMaxClientes();
	void setMaxClientes(int maxClientes);
	std::string getPuerto();
	void setPuerto(std::string puerto);
	std::vector<Usuario *> getUsuarios();
	void setUsuarios(std::vector<Usuario *>);
	void crearArchivoConfiguracion(std::string nombre);
};
#endif