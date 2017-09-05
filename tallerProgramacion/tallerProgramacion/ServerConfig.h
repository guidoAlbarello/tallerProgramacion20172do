#pragma once
#ifndef SERVERCONFIG_H
#define SERVERCONFIG_H

#include "Config.h"
#include "Usuario.h"
#include <vector>

using namespace std;

const std::string DEFAULT_SERVER_CONFIG = "server-config.xml";
const std::string DEFAULT_MAXCLIENTES = "3";
const std::string DEFAULT_PUERTO_SERVIDOR = "12345";

class ServerConfig : public Config {
public:
	ServerConfig();
	int getMaxClientes();
	void setMaxClientes(int maxClientes);
	std::string getPuerto();
	void setPuerto(std::string puerto);
	std::vector<Usuario *> getUsuarios();
	void setUsuarios(std::vector<Usuario *>);
private:
	int maxClientes;
	std::string puerto;
	std::vector<Usuario *> usuarios;
	void crearConfiguracionPredeterminada();
	void parsearArchivoXML(std::string nombre);
};
#endif