#include "ManejadorDeConexionServidor.h"

void ManejadorDeConexionServidor::iniciarConexion(std::string puertoEscucha, int cantidadConexionesMaxima) {
	this->socket->crearSocketServidor(puertoEscucha, cantidadConexionesMaxima);
}

SOCKET ManejadorDeConexionServidor::hayClienteIntentandoConectarse() {
	return this->socket->hayClienteIntentandoConectarse();
}
