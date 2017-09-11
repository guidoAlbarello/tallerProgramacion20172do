#pragma once
#ifndef CLIENTCONFIG_H
#define CLIENTCONFIG_H

#include "Config.h"
#include <string>

const std::string DEFAULT_CLIENT_CONFIG = "client-config.xml";
const std::string DEFAULT_IP = "127.0.0.1";
const std::string DEFAULT_PUERTO_CLIENTE = "15636";
const std::string DEFAULT_PATH = "/tmp/filetest_01";

class ClientConfig : public Config {
public:
	ClientConfig();
	std::string getPuerto();
	std::string getIP();
	std::string getPath();
	void setIP(std::string unaIP);
	void setPuerto(std::string unPuerto);
	void setPath(std::string path);
private:
	std::string IP;
	std::string puerto;
	std::string path;
	void crearConfiguracionPredeterminada();
	void parsearArchivoXML(std::string nombre);
};

#endif