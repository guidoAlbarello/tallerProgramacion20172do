#pragma once
#include <iostream>
using namespace std;

class Usuario {
	std::string nombre;
	std::string password;
public:
	Usuario(std::string nombre, std::string password);
	std::string getNombre();
	void setNombre(std::string nombre);
	std::string getPassword();
	void setPassword(std::string password);
private:

};