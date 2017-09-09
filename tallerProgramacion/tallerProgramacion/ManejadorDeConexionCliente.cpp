#include "ManejadorDeConexionCliente.h"
#include "Logger.h"
#include "Constantes.h"
#include "MensajeDeRed.h"

void ManejadorDeConexionCliente::iniciarConexion(std::string ipServidor, std::string puertoServidor) {
	this->socket->crearSocketCliente(ipServidor, puertoServidor);
	this->t_EnviarDatos = std::thread(&ManejadorDeConexionCliente::enviarDatos, this);
	this->t_RecibirDatos = std::thread(&ManejadorDeConexionCliente::recibirDatos, this);
}
bool ManejadorDeConexionCliente::login(std::string user, std::string pass) {
	MensajeDeRed *mensajeDeRed = new MensajeDeRed(LOG);
	mensajeDeRed->agregarParametro(user);
	mensajeDeRed->agregarParametro(pass);
	string mensaje = mensajeDeRed->getComandoSerializado();
	int tamanio = sizeof(mensaje);
	Logger::getInstance()->log(Debug, "enviando mensaje");
	Logger::getInstance()->log(Debug, mensaje);
	this->socket->enviarDatos(mensaje.c_str() , tamanio);

	return true;
}

