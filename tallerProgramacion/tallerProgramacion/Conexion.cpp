
#include "Conexion.h"


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

void Conexion::procesarPing() {
}

void Conexion::procesarSend_Message(MensajeDeRed* unMensajeDeRed) {
	if (unMensajeDeRed->getParametro(0).compare("") != 0) {
		Usuario* usuarioDestinatario = this->servidor->buscarUsuario(unMensajeDeRed->getParametro(0));
		this->getUsuario()->enviarMensaje(usuarioDestinatario, unMensajeDeRed->getParametro(1));
		Logger::getInstance()->log(Debug, "El envio de mensaje fue satisfactorio");
		ComandoCliente comando = ComandoCliente::RECIEVE_PRIVATE_MESSAGES;
		MensajeDeRed* mensajeDeRed = new MensajeDeRed(comando);
		mensajeDeRed->agregarParametro("RECIEVE_PRIVATE_MESSAGES_OK"); // ResultCode
		mensajeDeRed->agregarParametro(this->getUsuario()->getNombre());
		mensajeDeRed->agregarParametro(unMensajeDeRed->getParametro(1));
		string mensaje = mensajeDeRed->getComandoClienteSerializado();
		int tamanio = mensaje.length() + 1;
		Logger::getInstance()->log(Debug, "Enviando mensaje");
		Logger::getInstance()->log(Debug, mensaje);
		this->conexionConCliente->getSocket().enviarDatos(mensaje.c_str(), tamanio);
	} else {
		this->servidor->recibirMensajeGlobal(this->getUsuario()->getNombre(), unMensajeDeRed->getParametro(1));
		Logger::getInstance()->log(Debug, "El envio de mensaje fue satisfactorio");
		ComandoCliente comando = ComandoCliente::RESULTADO_SEND_MESSAGE;
		MensajeDeRed* mensajeDeRed = new MensajeDeRed(comando);
		mensajeDeRed->agregarParametro("SEND_MESSAGE_OK"); // ResultCode
		mensajeDeRed->agregarParametro("El envio de mensaje fue satisfactorio");
		string mensaje = mensajeDeRed->getComandoClienteSerializado();
		int tamanio = mensaje.length() + 1;
		Logger::getInstance()->log(Debug, "Enviando mensaje");
		Logger::getInstance()->log(Debug, mensaje);
		this->conexionConCliente->getSocket().enviarDatos(mensaje.c_str(), tamanio);
	}
	
}

void Conexion::procesarRetrieve_Messages(MensajeDeRed* unMensajeDeRed) {
	ComandoCliente comando = ComandoCliente::RESULTADO_RETRIEVE_MESSAGES;
	MensajeDeRed* mensajeDeRed = new MensajeDeRed(comando);
	mensajeDeRed->agregarParametro("RESULTADO_RETRIEVE_MESSAGES_OK"); // ResultCode
	unsigned int tamanioBuzon = this->getUsuario()->getBuzon()->getTamanio();
	BuzonDeMensajes* buzonDeUsuario = this->getUsuario()->getBuzon();
	for (unsigned int i = 0; i < tamanioBuzon; i++) {
		Mensaje* unMensaje = buzonDeUsuario->verMensaje(i);
		mensajeDeRed->agregarParametro(unMensaje->getEmisor());
		mensajeDeRed->agregarParametro(unMensaje->getMensaje());
	}

	string mensaje = mensajeDeRed->getComandoClienteSerializado();
	int tamanio = mensaje.length() + 1;
	Logger::getInstance()->log(Debug, "Enviando mensaje");
	Logger::getInstance()->log(Debug, mensaje);
	if (this->conexionConCliente->getSocket().enviarDatos(mensaje.c_str(), tamanio)) {
		buzonDeUsuario->eliminarMensajes(tamanioBuzon);
	}
}

void Conexion::enviarChatGlobal(string unEmisor, string unMensaje) {
	Logger::getInstance()->log(Debug, "El envio de mensaje fue satisfactorio");
	ComandoCliente comando = ComandoCliente::RECIEVE_GLOBAL_MESSAGES;
	MensajeDeRed* mensajeDeRed = new MensajeDeRed(comando);
	mensajeDeRed->agregarParametro("RECIEVE_GLOBAL_MESSAGES_OK"); // ResultCode
	mensajeDeRed->agregarParametro(unEmisor);
	mensajeDeRed->agregarParametro(unMensaje);
	string mensaje = mensajeDeRed->getComandoClienteSerializado();
	int tamanio = mensaje.length() + 1;
	Logger::getInstance()->log(Debug, "Enviando mensaje");
	Logger::getInstance()->log(Debug, mensaje);
	this->conexionConCliente->getSocket().enviarDatos(mensaje.c_str(), tamanio);
}

void Conexion::procesarDatosRecibidos() {
	while (conexionActiva) {
		char* datosRecibidos = this->conexionConCliente->getDatosRecibidos();
		if (datosRecibidos != NULL) {
			Logger::getInstance()->log(Debug, datosRecibidos);
			std::string datosRecibidosString(datosRecibidos);
			MensajeDeRed *mensajeDeRed = new MensajeDeRed(datosRecibidosString, Constantes::SERVIDOR);
			Usuario* unUsuario = NULL;
			/* Procesa comandos recibidos desde los clientes */
			switch (mensajeDeRed->getComandoServidor()) {
			case ComandoServidor::LOG:
				Logger::getInstance()->log(Debug, "Recibio un LOG");
				// Envia respuesta con el resultado del login
				unUsuario = this->servidor->validarLogin(mensajeDeRed);
				if (unUsuario != NULL) {
					this->usuarioConectado = unUsuario;
					Logger::getInstance()->log(Debug, "El login fue satisfactorio");
					ComandoCliente comando = ComandoCliente::RESULTADO_LOGIN;
					MensajeDeRed* mensajeDeRed = new MensajeDeRed(comando);
					mensajeDeRed->agregarParametro("LOGIN_OK"); // ResultCode
					mensajeDeRed->agregarParametro("El login fue satisfactorio");
					string mensaje = mensajeDeRed->getComandoClienteSerializado();
					int tamanio = mensaje.length() + 1;
					Logger::getInstance()->log(Debug, "Enviando mensaje");
					Logger::getInstance()->log(Debug, mensaje);
					this->conexionConCliente->getSocket().enviarDatos(mensaje.c_str(), tamanio);
				} else {
					Logger::getInstance()->log(Debug, "Login invalido");
					ComandoCliente comando = ComandoCliente::RESULTADO_LOGIN;
					MensajeDeRed* mensajeDeRed = new MensajeDeRed(comando);
					mensajeDeRed->agregarParametro("LOGIN_NOK"); // ResultCode
					mensajeDeRed->agregarParametro("Login invalido");
					string mensaje = mensajeDeRed->getComandoClienteSerializado();
					int tamanio = mensaje.length() + 1;
					Logger::getInstance()->log(Debug, "Login invalido");
					Logger::getInstance()->log(Debug, mensaje);
					this->conexionConCliente->getSocket().enviarDatos(mensaje.c_str(), tamanio);
				}
				break;
			case ComandoServidor::PING:
				Logger::getInstance()->log(Debug, "Recibio un PING");
				procesarPing();
				break;
			case ComandoServidor::SEND_MESSAGE:
				Logger::getInstance()->log(Debug, "Recibio un Send_message");
				procesarSend_Message(mensajeDeRed);
				break;
			case ComandoServidor::RETRIEVE_MESSAGES:
				Logger::getInstance()->log(Debug, "Recibio un Retrieve_message");
				procesarRetrieve_Messages(mensajeDeRed);
				break;
			default:
				Logger::getInstance()->log(Debug, datosRecibidos);
			}
			this->conexionConCliente->setDatosRecibidos(NULL);
		}
	}
}

