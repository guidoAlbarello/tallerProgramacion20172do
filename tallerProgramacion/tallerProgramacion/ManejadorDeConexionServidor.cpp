#include "ManejadorDeConexionServidor.h"

void ManejadorDeConexionServidor::iniciarConexion(std::string puertoEscucha, int cantidadConexionesMaxima) {
	this->socket->crearSocketServidor(puertoEscucha, cantidadConexionesMaxima);
}

SOCKET ManejadorDeConexionServidor::hayClienteIntentandoConectarse(size_t conexionesActivas, int maxClientes) {
	if (conexionesActivas == maxClientes) {
		return INVALID_SOCKET;
	}

	return this->socket->hayClienteIntentandoConectarse();
}
