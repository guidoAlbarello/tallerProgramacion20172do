#include "ManejadorDeConexionCliente.h"
#include "Logger.h"
#include "Constantes.h"
#include "MensajeDeRed.h"
#include <iostream>

bool ManejadorDeConexionCliente::iniciarConexion(std::string ipServidor, std::string puertoServidor) {
	int resultadoConexion = this->socket->crearSocketCliente(ipServidor, puertoServidor);
	this->t_EnviarDatos = std::thread(&ManejadorDeConexionCliente::enviarDatos, this);
	this->t_RecibirDatos = std::thread(&ManejadorDeConexionCliente::recibirDatos, this);
	if (resultadoConexion == 0) {
		Logger::getInstance()->log(Actividad, "El cliente con ip = " + ipServidor + " se ha conectado satisfactoriamente al servidor");
		
		this->conexionActiva = true;
		return true;
	} else {
		Logger::getInstance()->log(Error, "El cliente con ip = " + ipServidor + " no se pudo conectar al servidor");
		cout << "Fallo la conexion al servidor" << endl;
		this->conexionActiva = false;
		return false;
	}
}
bool ManejadorDeConexionCliente::login(std::string user, std::string pass) {
	Logger::getInstance()->log(Debug, "se realiza el envio de un comando LOG");
	MensajeDeRed *mensajeDeRed = new MensajeDeRed(ComandoServidor::LOG);
	mensajeDeRed->agregarParametro(user);
	mensajeDeRed->agregarParametro(pass);
	string mensaje = mensajeDeRed->getComandoServidorSerializado();
	int tamanio = mensaje.length() + 1;
	Logger::getInstance()->log(Debug, mensaje);
	
	return this->socket->enviarDatos(mensaje.c_str(), tamanio);
}


bool ManejadorDeConexionCliente::enviarMensajeGlobal(string unMensaje) {
	Logger::getInstance()->log(Debug, "se realiza el envio de un comando SEND_MESSAGES para mensaje global");
	MensajeDeRed *mensajeDeRed = new MensajeDeRed(ComandoServidor::SEND_MESSAGE);
	mensajeDeRed->agregarParametro("");
	mensajeDeRed->agregarParametro(unMensaje);
	string mensaje = mensajeDeRed->getComandoServidorSerializado();
	int tamanio = mensaje.length() + 1;;
	Logger::getInstance()->log(Debug, mensaje);
	return this->socket->enviarDatos(mensaje.c_str(), tamanio);;
}

bool ManejadorDeConexionCliente::desconectarServidor() {
	MensajeDeRed* mensajeDeRed = new MensajeDeRed(ComandoServidor::CLOSE);
	//Logger::getInstance()->log(Debug, "se realiza el envio de un PING");
	string mensaje = mensajeDeRed->getComandoServidorSerializado();
	int tamanio = mensaje.length() + 1;
	Logger::getInstance()->log(Debug, mensaje);
	return this->socket->enviarDatos(mensaje.c_str(), tamanio);;
}

bool ManejadorDeConexionCliente::enviarMensajePrivado(string unDestinatario, string unMensaje) {
	Logger::getInstance()->log(Debug, "se realiza el envio de un comando SEND_MESSAGES para mensaje privado");
	MensajeDeRed *mensajeDeRed = new MensajeDeRed(ComandoServidor::SEND_MESSAGE);
	mensajeDeRed->agregarParametro(unDestinatario);
	mensajeDeRed->agregarParametro(unMensaje);
	string mensaje = mensajeDeRed->getComandoServidorSerializado();
	int tamanio = mensaje.length() + 1;
	Logger::getInstance()->log(Debug, mensaje);
	return this->socket->enviarDatos(mensaje.c_str(), tamanio);;
}

bool ManejadorDeConexionCliente::realizarPeticionUsuarios() {

	MensajeDeRed *mensajeDeRed = new MensajeDeRed(ComandoServidor::USUARIOS);
	string mensaje = mensajeDeRed->getComandoServidorSerializado();
	int tamanio = mensaje.length() + 1;
	Logger::getInstance()->log(Debug, "se realiza el envio de un comando USUARIOS");
	return this->socket->enviarDatos(mensaje.c_str(), tamanio);;
}

bool ManejadorDeConexionCliente::devolverMensajesPrivados() {
	Logger::getInstance()->log(Debug, "se realiza el envio de un RETRIEVE_MESSAGES");
	MensajeDeRed *mensajeDeRed = new MensajeDeRed(ComandoServidor::RETRIEVE_MESSAGES);
	string mensaje = mensajeDeRed->getComandoServidorSerializado();
	int tamanio = mensaje.length() + 1;
	Logger::getInstance()->log(Debug, mensaje);
	return this->socket->enviarDatos(mensaje.c_str(), tamanio);;
}
void ManejadorDeConexionCliente::borrarEntorno() {
	this->socket->borrarEntornoWSA();
}

bool ManejadorDeConexionCliente::enviarSolicitudPing() {
	MensajeDeRed* mensajeDeRed = new MensajeDeRed(ComandoServidor::HPING);
	//Logger::getInstance()->log(Debug, "se realiza el envio de un PING");
	string mensaje = mensajeDeRed->getComandoServidorSerializado();
	int tamanio = mensaje.length() + 1;
	Logger::getInstance()->log(Debug, mensaje);
	return this->socket->enviarDatos(mensaje.c_str(), tamanio);;
}

bool ManejadorDeConexionCliente::enviarEntrada() {
	
	//Logger::getInstance()->log(Debug, "Enviando entrada");

	bool teclas[Constantes::CANT_TECLAS];
	teclas[0] = ManejadorInput::getInstance()->estaTeclaPresionada(SDL_SCANCODE_UP);
	teclas[1] = ManejadorInput::getInstance()->estaTeclaPresionada(SDL_SCANCODE_RIGHT);
	teclas[2] = ManejadorInput::getInstance()->estaTeclaPresionada(SDL_SCANCODE_LEFT);
	teclas[3] = ManejadorInput::getInstance()->estaTeclaPresionada(SDL_SCANCODE_N);
	int tamanio = (Constantes::CANT_TECLAS * sizeof(bool) ) + 5 + 1;
	
	char* data = new char[tamanio];
	std::string strComando = "INPUT";
	strComando.append(&Constantes::separador);
	const char* comando = strComando.c_str();
	memcpy(data, comando, 6);
	memcpy(data + 6, teclas, sizeof(bool) * Constantes::CANT_TECLAS);
	bool resultado = this->socket->enviarDatos((char*)data, tamanio);;

	if (data != NULL)
		free(data);

	return resultado;
}
