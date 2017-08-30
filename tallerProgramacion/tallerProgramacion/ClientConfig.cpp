#include "ClientConfig.h"

ClientConfig::ClientConfig() {

}

ClientConfig::ClientConfig(std::string unaIP, int unPuerto, std::string path) {
	this->IP = unaIP;
	this->puerto = unPuerto;
	this->path = path;
}

void ClientConfig::setPuerto(int unPuerto) {
	this->puerto = unPuerto;
}

void ClientConfig::setIP(std::string unaIP) {
	this->IP = unaIP;
}

void ClientConfig::setPath(std::string path) {
	this->path = path;
}

int ClientConfig::getPuerto() {
	return this->puerto;
}

std::string ClientConfig::getIP() {
	return this->IP;
}

std::string ClientConfig::getPath() {
	return this->path;
}