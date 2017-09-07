#include "ManejadorDeConexionCliente.h"
#include "Logger.h"

void ManejadorDeConexionCliente::iniciarConexion(std::string ipServidor, std::string puertoServidor) {
	this->socket->crearSocketCliente(ipServidor, puertoServidor);
	this->t_EnviarDatos = std::thread(&ManejadorDeConexionCliente::enviarDatos, this);
	this->t_RecibirDatos = std::thread(&ManejadorDeConexionCliente::recibirDatos, this);
}
bool ManejadorDeConexionCliente::login(std::string user, std::string pass) {
	int tamanio = user.length() + 1 + pass.length();
	string mensaje = user + '-' + pass;
	Logger::getInstance()->log(Debug, "enviando mensaje");
	Logger::getInstance()->log(Debug, mensaje);
	this->socket->enviarDatos(mensaje.c_str() , tamanio);

	return true;
}

