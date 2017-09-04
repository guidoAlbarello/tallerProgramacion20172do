#pragma once
#ifndef CLIENTCONFIG_H
#define CLIENTCONFIG_H

#include <string>

const std::string DEFAULT_IP = "127.0.0.1";
const std::string DEFAULT_PUERTO_CLIENTE = "15636";
const std::string DEFAULT_TESTFILE = "/tmp/filetest_01";

class ClientConfig {
	std::string IP;
	std::string puerto;
	std::string path;
public:
	ClientConfig();
	ClientConfig(std::string unaIP, std::string unPuerto, std::string path);
	std::string getPuerto();
	std::string getIP();
	std::string getPath();
	void setIP(std::string unaIP);
	void setPuerto(std::string unPuerto);
	void setPath(std::string path);
	void crearArchivoConfiguracion(std::string nombre);
};

#endif