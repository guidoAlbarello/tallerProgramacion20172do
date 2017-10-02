#include "Usuario.h"
#include <iostream>
using namespace std;


Usuario::Usuario() {
}

Usuario::Usuario(std::string name, std::string pass) {
	this->nombre = name;
	this->password = pass;

	this->buzonDeMensajes = new BuzonDeMensajes();
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

void Usuario::enviarMensaje(Usuario* unUsuario, std::string unMensaje) {
	unUsuario->recibirMensaje(unUsuario->getNombre(), this->getNombre(), unMensaje);						//tirar excepcion si usuario == NULL
}

void Usuario::recibirMensaje(std::string unDestinatario, std::string unEmisor, std::string unMensaje) {
	buzonDeMensajes->recibirMensaje(unDestinatario, unEmisor, unMensaje);
}

