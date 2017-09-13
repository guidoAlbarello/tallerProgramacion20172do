#include "ManejadorDeConexionServidor.h"
#include "Logger.h"
#include <iostream>

void ManejadorDeConexionServidor::iniciarConexion(std::string puertoEscucha, int cantidadConexionesMaxima) {
	int resultadoConexion = this->socket->crearSocketServidor(puertoEscucha, cantidadConexionesMaxima);
	if (resultadoConexion == 0) {
		cout << "Servidor iniciado correctamente" << endl;
		Logger::getInstance()->log(Debug, "Se creo correctamente el socket del servidor escuchando en el puerto = " + puertoEscucha);
	}
	else {
		cout << "Fallo la inicializacion del servidor" << endl;
		Logger::getInstance()->log(Debug, "Fallo la creacion del socket del servidor escuchando en el puerto = " + puertoEscucha);

	}
}

SOCKET ManejadorDeConexionServidor::hayClienteIntentandoConectarse(size_t conexionesActivas, int maxClientes) {
	if (conexionesActivas == maxClientes) {
		Logger::getInstance()->log(Debug, "Un cliente no se pudo conectar debido a que se llego al maximo de conexiones aceptadas");
		return INVALID_SOCKET;
	}

	return this->socket->hayClienteIntentandoConectarse();
}
