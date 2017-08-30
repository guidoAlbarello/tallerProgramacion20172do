#pragma once
#ifndef SERVERCONFIG_H
#define SERVERCONFIG_H

//#include <iostream>
#include "Usuario.h"
#include <vector>

class ServerConfig {
	int maxClientes;
	int puerto;
	std::vector<Usuario> usuarios;
public:
	ServerConfig();
	ServerConfig(int maxClientes, int puerto, std::vector<Usuario> usuarios);
	int getMaxClientes();
	void setMaxClientes(int maxClientes);
	int getPuerto();
	void setPuerto(int puerto);
	std::vector<Usuario> getUsuarios();
	void setUsuarios(std::vector<Usuario>);
};
#endif