#include "ManejadorDeConexionCliente.h"

void ManejadorDeConexionCliente::iniciarConexion(std::string ipServidor, std::string puertoServidor) {
	this->socket->crearSocketCliente(ipServidor, puertoServidor);

	this->t_EnviarDatos = std::thread(&ManejadorDeConexionCliente::enviarDatos, this);
	this->t_RecibirDatos = std::thread(&ManejadorDeConexionCliente::recibirDatos, this);
}

char* ManejadorDeConexionCliente::procesarDatosAEnviar() {
	return NULL;
}

char* ManejadorDeConexionCliente::procesarDatosRecibidos(char* datosRecibidos) {
	return NULL;
}
