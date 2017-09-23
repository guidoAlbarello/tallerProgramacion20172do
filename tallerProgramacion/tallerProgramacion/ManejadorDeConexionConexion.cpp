#include "ManejadorDeConexionConexion.h"



ManejadorDeConexionConexion::ManejadorDeConexionConexion(SOCKET unSocket) {
	ManejadorDeConexion::ManejadorDeConexion(unSocket);
	this->socket = new SocketSincronico(unSocket);
}


void ManejadorDeConexionConexion::iniciarConexion() {
	this->t_EnviarDatos = std::thread(&ManejadorDeConexionConexion::enviarDatos, this);
	this->t_RecibirDatos = std::thread(&ManejadorDeConexionConexion::recibirDatos, this);
}
