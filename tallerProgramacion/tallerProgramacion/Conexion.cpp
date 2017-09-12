#include "ParserDeMensajes.h"
#include "Conexion.h"
#include "Servidor.h"

Conexion::Conexion(SOCKET unSocket, Servidor* servidor) {
	this->servidor = servidor;
	this->conexionConCliente = new ManejadorDeConexionConexion(unSocket);
	this->conexionConCliente->iniciarConexion();
	this->conexionActiva = true;
	this->usuarioConectado = NULL;
	this->t_procesarDatosRecibidos = std::thread(&Conexion::procesarDatosRecibidos, this);
}

void Conexion::cerrarConexion() {
	this->conexionActiva = false;

	if (t_procesarDatosRecibidos.joinable()) {
		t_procesarDatosRecibidos.join();
	}
	this->getConexionConCliente()->cerrarConexion();
}

void Conexion::procesarDatosRecibidos() {
	while (conexionActiva) {
		//procesar comandos
		char* datosRecibidos = this->conexionConCliente->getDatosRecibidos();
		if (datosRecibidos != NULL) {
			Logger::getInstance()->log(Debug, datosRecibidos);
			std::string datosRecibidosString(datosRecibidos);
			MensajeDeRed *mensajeDeRed = new MensajeDeRed(datosRecibidosString);
			
			switch (mensajeDeRed->getComando())
			{
			case LOG:
				Logger::getInstance()->log(Debug, "Recibio un LOG");
				if (this->servidor->validarLogin(mensajeDeRed)) {
					Logger::getInstance()->log(Debug, "Login satisfactorio");
					//std::cout << "El login fue satisfactorio" << endl;
					this->conexionConCliente->setDatosRecibidos(NULL);

					// Envio una respuesta con el resultado del login
					//MensajeDeRed* mensajeDeRed = new MensajeDeRed("El login fue satisfactorio");
					//string mensaje = mensajeDeRed->getComandoSerializado();
					std::string mensaje = "Cliente: El login fue satisfactorio";
					int tamanio = sizeof(mensaje);
					Logger::getInstance()->log(Debug, "Enviando mensaje");
					Logger::getInstance()->log(Debug, mensaje);
					this->conexionConCliente->getSocket().enviarDatos(mensaje.c_str(), tamanio);
				}
				else {
					std::cout << "Login invalido" << endl;

				}
				break;
			case PING:
				Logger::getInstance()->log(Debug, "Recibio un PING");
				break;
			case SEND_MESSAGE:
				Logger::getInstance()->log(Debug, "Recibio un Send_message");
				break;
			case RETRIEVE_MESSAGES:
				Logger::getInstance()->log(Debug, "Recibio un Retrieve_message");
				break;
			default:
				Logger::getInstance()->log(Debug, datosRecibidos);
			}
			
		}
	}
}

