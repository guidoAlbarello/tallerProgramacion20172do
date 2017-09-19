#include "ManejadorDeConexionCliente.h"
#include "Logger.h"
#include "Constantes.h"
#include "MensajeDeRed.h"
#include <iostream>

void ManejadorDeConexionCliente::iniciarConexion(std::string ipServidor, std::string puertoServidor) {
	int resultadoConexion = this->socket->crearSocketCliente(ipServidor, puertoServidor);
	this->t_EnviarDatos = std::thread(&ManejadorDeConexionCliente::enviarDatos, this);
	this->t_RecibirDatos = std::thread(&ManejadorDeConexionCliente::recibirDatos, this);
	if (resultadoConexion == 0) {
		Logger::getInstance()->log(Debug, "El cliente con ip = " + ipServidor + " se ha conectado satisfactoriamente al servidor");
		cout << "Se ha conectado satisfactoriamente al servidor" << endl;
		this->conexionActiva = true;
	}
	else {
		Logger::getInstance()->log(Debug, "El cliente con ip = " + ipServidor + " no se pudo conectar al servidor");
		cout << "Fallo la conexion al servidor" << endl;
		this->conexionActiva = false;
	}
}
bool ManejadorDeConexionCliente::login(std::string user, std::string pass) {
	MensajeDeRed *mensajeDeRed = new MensajeDeRed(ComandoServidor::LOG);
	mensajeDeRed->agregarParametro(user);
	mensajeDeRed->agregarParametro(pass);
	string mensaje = mensajeDeRed->getComandoServidorSerializado();
	int tamanio = mensaje.length() +1;
	Logger::getInstance()->log(Debug, mensaje);
	this->socket->enviarDatos(mensaje.c_str() , tamanio);

	return true;
}

bool ManejadorDeConexionCliente::enviarMensajeGlobal(string unMensaje) {
	MensajeDeRed *mensajeDeRed = new MensajeDeRed(ComandoServidor::SEND_MESSAGE);
	mensajeDeRed->agregarParametro("");
	mensajeDeRed->agregarParametro(unMensaje);
	string mensaje = mensajeDeRed->getComandoServidorSerializado();
	int tamanio = mensaje.length() +1;;
	Logger::getInstance()->log(Debug, mensaje);
	return this->socket->enviarDatos(mensaje.c_str(), tamanio);;
}

bool ManejadorDeConexionCliente::enviarMensajePrivado(string unDestinatario, string unMensaje) {
	MensajeDeRed *mensajeDeRed = new MensajeDeRed(ComandoServidor::SEND_MESSAGE);
	mensajeDeRed->agregarParametro(unDestinatario);
	mensajeDeRed->agregarParametro(unMensaje);
	string mensaje = mensajeDeRed->getComandoServidorSerializado();
	int tamanio = mensaje.length() +1;
	Logger::getInstance()->log(Debug, mensaje);
	return this->socket->enviarDatos(mensaje.c_str(), tamanio);;
}

bool ManejadorDeConexionCliente::devolverMensajesPrivados() {
	MensajeDeRed *mensajeDeRed = new MensajeDeRed(ComandoServidor::RETRIEVE_MESSAGES);
	string mensaje = mensajeDeRed->getComandoServidorSerializado();
	int tamanio = mensaje.length() +1;
	Logger::getInstance()->log(Debug, mensaje);
	return this->socket->enviarDatos(mensaje.c_str(), tamanio);;
};

bool ManejadorDeConexionCliente::enviarSolicitudPing() {
	MensajeDeRed* mensajeDeRed = new MensajeDeRed(ComandoServidor::PING);
	string mensaje = mensajeDeRed->getComandoServidorSerializado();
	int tamanio = mensaje.length() + 1;
	Logger::getInstance()->log(Debug, mensaje);
	return this->socket->enviarDatos(mensaje.c_str(), tamanio);;
}

bool ManejadorDeConexionCliente::enviarRespuestaPingAServidor() {
	MensajeDeRed* mensajeDeRed = new MensajeDeRed(ComandoServidor::RESULTADO_PING);
	string mensaje = mensajeDeRed->getComandoServidorSerializado();
	int tamanio = mensaje.length() + 1;
	Logger::getInstance()->log(Debug, mensaje);
	return this->socket->enviarDatos(mensaje.c_str(), tamanio);;
}

