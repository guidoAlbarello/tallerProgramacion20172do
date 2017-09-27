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
		Logger::getInstance()->log(Error, "Fallo la creacion del socket del servidor escuchando en el puerto = " + puertoEscucha);

	}
}

SOCKET ManejadorDeConexionServidor::hayClienteIntentandoConectarse(size_t conexionesActivas, int maxClientes) {
	SOCKET socketAceptado = this->socket->hayClienteIntentandoConectarse();
	// Cuando se intenta conectar un cliente que excede el maximo el conexionesActivas no esta incrementado todavia
	if (conexionesActivas + 1 > maxClientes) {
		Logger::getInstance()->log(Debug, "Un cliente intento conectarse pero se excedio el maximo de conexiones aceptadas");
		return INVALID_SOCKET;
	}
	return socketAceptado;
}

void ManejadorDeConexionServidor::borrarEntorno() {
	this->socket->borrarEntornoWSA();
}

