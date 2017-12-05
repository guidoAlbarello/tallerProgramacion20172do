#include "ManejadorDeConexionConexion.h"



ManejadorDeConexionConexion::ManejadorDeConexionConexion(SOCKET unSocket) {
	ManejadorDeConexion::ManejadorDeConexion(unSocket);
	this->socket = new SocketSincronico(unSocket);
}

bool ManejadorDeConexionConexion::cerrarCliente() {
	ComandoCliente comando = ComandoCliente::CLOSE;
	MensajeDeRed* mensajeDeRed = new MensajeDeRed(comando);
	string mensaje = mensajeDeRed->getComandoClienteSerializado();
	int tamanio = mensaje.length() + 1;
	Logger::getInstance()->log(Debug, mensaje);
	Logger::getInstance()->log(Debug, "Enviando respuesta de ping a cliente");

	return this->getSocket().enviarDatos(mensaje.c_str(), tamanio);
}


void ManejadorDeConexionConexion::iniciarConexion() {
	this->t_EnviarDatos = std::thread(&ManejadorDeConexionConexion::enviarDatos, this);
	this->t_RecibirDatos = std::thread(&ManejadorDeConexionConexion::recibirDatos, this);
}
