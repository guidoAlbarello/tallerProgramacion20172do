#pragma once
#ifndef CLIENTCONFIG_H
#define CLIENTCONFIG_H

#include <string>

class ClientConfig {
	std::string IP;
	int puerto;
	std::string path;
public:
	ClientConfig();
	ClientConfig(std::string unaIP, int unPuerto, std::string path);
	int getPuerto();
	std::string getIP();
	std::string getPath();
	void setIP(std::string unaIP);
	void setPuerto(int unPuerto);
	void setPath(std::string path);
};

#endif