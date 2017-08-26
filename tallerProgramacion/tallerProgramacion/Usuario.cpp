#include "Usuario.h"
#include <iostream>
using namespace std;

Usuario::Usuario(std::string name, std::string pass) {
	this->nombre = name;
	this->password = pass;
}

std::string Usuario::getNombre() {
	return this->nombre;
}

void Usuario::setNombre(std::string nombre) {
	this->nombre = nombre;
}

std::string Usuario::getPassword() {
	return this->password;
}

void Usuario::setPassword(std::string password) {
	this->password = password;
}
