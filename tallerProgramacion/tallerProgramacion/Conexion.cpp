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
		char* datosRecibidos = this->conexionConCliente->getDatosRecibidos();
		if (datosRecibidos != NULL) {
			Logger::getInstance()->log(Debug, datosRecibidos);
			std::string datosRecibidosString(datosRecibidos);
			MensajeDeRed *mensajeDeRed = new MensajeDeRed(datosRecibidosString, Constantes::SERVIDOR);

			/* Procesa comandos recibidos desde los clientes */
			switch (mensajeDeRed->getComandoServidor())
			{
			case ComandoServidor::LOG:
				Logger::getInstance()->log(Debug, "Recibio un LOG");
				// Envia respuesta con el resultado del login
				if (this->servidor->validarLogin(mensajeDeRed)) {
					Logger::getInstance()->log(Debug, "El login fue satisfactorio");
					ComandoCliente comando = ComandoCliente::RESULTADO_LOGIN;
					MensajeDeRed* mensajeDeRed = new MensajeDeRed(comando);
					mensajeDeRed->agregarParametro("LOGIN_OK"); // ResultCode
					mensajeDeRed->agregarParametro("El login fue satisfactorio");
					string mensaje = mensajeDeRed->getComandoClienteSerializado();
					int tamanio = sizeof(mensaje);
					Logger::getInstance()->log(Debug, "Enviando mensaje");
					Logger::getInstance()->log(Debug, mensaje);
					this->conexionConCliente->getSocket().enviarDatos(mensaje.c_str(), tamanio);
				}
				else {
					Logger::getInstance()->log(Debug, "Login invalido");
					ComandoCliente comando = ComandoCliente::RESULTADO_LOGIN;
					MensajeDeRed* mensajeDeRed = new MensajeDeRed(comando);
					mensajeDeRed->agregarParametro("LOGIN_NOK"); // ResultCode
					mensajeDeRed->agregarParametro("Login invalido");
					string mensaje = mensajeDeRed->getComandoClienteSerializado();
					int tamanio = sizeof(mensaje);
					Logger::getInstance()->log(Debug, "Login invalido");
					Logger::getInstance()->log(Debug, mensaje);
					this->conexionConCliente->getSocket().enviarDatos(mensaje.c_str(), tamanio);
				}
				break;
			case ComandoServidor::PING:
				Logger::getInstance()->log(Debug, "Recibio un PING");
				break;
			case ComandoServidor::SEND_MESSAGE:
				Logger::getInstance()->log(Debug, "Recibio un Send_message");
				break;
			case ComandoServidor::RETRIEVE_MESSAGES:
				Logger::getInstance()->log(Debug, "Recibio un Retrieve_message");
				break;
			default:
				Logger::getInstance()->log(Debug, datosRecibidos);
			}
			this->conexionConCliente->setDatosRecibidos(NULL);			
		}
	}
}

