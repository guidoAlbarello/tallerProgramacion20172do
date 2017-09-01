#include "ManejadorDeConexionServidor.h"

void ManejadorDeConexionServidor::iniciarConexion(std::string puertoEscucha, int cantidadConexionesMaxima) {
	this->socket->crearSocketServidor(puertoEscucha, cantidadConexionesMaxima);

	this->t_EnviarDatos = std::thread(&ManejadorDeConexionServidor::enviarDatos, this);
	this->t_RecibirDatos = std::thread(&ManejadorDeConexionServidor::recibirDatos, this);
}

char* ManejadorDeConexionServidor::procesarDatosAEnviar() {
	return NULL;
}

char* ManejadorDeConexionServidor::procesarDatosRecibidos(char* datosRecibidos) {
	return NULL;
}
