#include "ServerConfig.h"
#include <vector>
#include "Usuario.h"

ServerConfig::ServerConfig() {

}

ServerConfig::ServerConfig(int maxClientes, int puerto, std::vector<Usuario> usuarios) {
	this->maxClientes = maxClientes;
	this->puerto = puerto;
	this->usuarios = usuarios;
}

int ServerConfig::getMaxClientes() {
	return this->maxClientes;
}

void ServerConfig::setMaxClientes(int maxClientes) {
	this->maxClientes = maxClientes;
}

int ServerConfig::getPuerto() {
	return this->puerto;
}
void ServerConfig::setPuerto(int puerto) {
	this->puerto = puerto;
}

std::vector<Usuario> ServerConfig::getUsuarios() {
	return this->usuarios;
}

void ServerConfig::setUsuarios(std::vector<Usuario> usuarios) {
	this->usuarios = usuarios;
}
