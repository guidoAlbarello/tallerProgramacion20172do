
#include "Conexion.h"


Conexion::Conexion(SOCKET unSocket, Servidor* servidor) {
	this->servidor = servidor;
	this->conexionConCliente = new ManejadorDeConexionConexion(unSocket);
	this->conexionConCliente->iniciarConexion();
	this->conexionActiva = true;
	this->conexionViva = true;
	this->conexionCerrada = false;
	this->usuarioConectado = NULL;
	this->t_procesarDatosRecibidos = std::thread(&Conexion::procesarDatosRecibidos, this);
}

Conexion::~Conexion() {

}

void Conexion::cerrarConexion() {
	this->conexionActiva = false;
	this->conexionViva = false;
	this->conexionCerrada = true;
	try {
		if (t_procesarDatosRecibidos.joinable()) {
			t_procesarDatosRecibidos.join();
		}

		if (this->getConexionConCliente() != NULL) {
			this->getConexionConCliente()->cerrarConexion();
		}
	} catch (exception e) {
	}

	
}

void Conexion::procesarSolicitudPing() {
	// Envia un ping de vuelta hacia el cliente como respuesta
	ComandoCliente comando = ComandoCliente::RESULTADO_PING;
	MensajeDeRed* mensajeDeRed = new MensajeDeRed(comando);
	string mensaje = mensajeDeRed->getComandoClienteSerializado();
	int tamanio = mensaje.length() + 1;
	Logger::getInstance()->log(Debug, mensaje);
	Logger::getInstance()->log(Debug, "Enviando respuesta de ping a cliente");
	this->conexionConCliente->getSocket().enviarDatos(mensaje.c_str(), tamanio);
}

void Conexion::procesarSend_Message(MensajeDeRed* unMensajeDeRed) {
	if (unMensajeDeRed->getParametro(0).compare("") != 0) {
		Usuario* usuarioDestinatario = this->servidor->buscarUsuario(unMensajeDeRed->getParametro(0));
		if (usuarioDestinatario == NULL) {
			Logger::getInstance()->log(Debug, "No existe el usuario" + unMensajeDeRed->getParametro(0) + "para enviar el mensaje");
			ComandoCliente comando = ComandoCliente::RESULTADO_SEND_MESSAGE;
			MensajeDeRed* mensajeDeRed = new MensajeDeRed(comando);
			mensajeDeRed->agregarParametro("SEND_MESSAGE_NOK"); // ResultCode
			mensajeDeRed->agregarParametro("No existe el usuario para enviar el mensaje");
			string mensaje = mensajeDeRed->getComandoClienteSerializado();
			int tamanio = mensaje.length() + 1;
			Logger::getInstance()->log(Debug, "Enviando mensaje");
			Logger::getInstance()->log(Debug, mensaje);
			this->conexionConCliente->getSocket().enviarDatos(mensaje.c_str(), tamanio);
		} else {
			this->getUsuario()->enviarMensaje(usuarioDestinatario, unMensajeDeRed->getParametro(1));
			this->servidor->enviarMensajePrivado(this->getUsuario()->getNombre(), unMensajeDeRed->getParametro(1));
			ComandoCliente comando = ComandoCliente::RESULTADO_SEND_MESSAGE;
			MensajeDeRed* mensajeDeRed = new MensajeDeRed(comando);
			mensajeDeRed->agregarParametro("SEND_MESSAGE_OK"); // ResultCode
			mensajeDeRed->agregarParametro("El envio del mensaje fue satisfactorio");
			string mensaje = mensajeDeRed->getComandoClienteSerializado();
			int tamanio = mensaje.length() + 1;
			Logger::getInstance()->log(Debug, "Enviando mensaje");
			Logger::getInstance()->log(Debug, mensaje);
			this->conexionConCliente->getSocket().enviarDatos(mensaje.c_str(), tamanio);

		}
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

void Conexion::procesarPeticionListaDeUsuarios() {
	ComandoCliente comando = ComandoCliente::RESULTADO_USUARIOS;
	MensajeDeRed* mensajeDeRed = new MensajeDeRed(comando);

	std::vector<Conexion*> conexionesActivas = this->servidor->getConexionesActivas();

	if (conexionesActivas.size() == 0) {

		mensajeDeRed->agregarParametro("no hay ususarios conectados en este momento");
		Logger::getInstance()->log(Debug, "no habian ususarios conectados apra mostrar");
	} else {
		for (unsigned int i = 0; i < conexionesActivas.size(); i++) {
			if (conexionesActivas[i]->getUsuario() != NULL) {
				string unUsuario = "Usuario: " + conexionesActivas[i]->getUsuario()->getNombre();
				mensajeDeRed->agregarParametro(unUsuario);
			} else {
				Logger::getInstance()->log(Debug, "Usuario conectado sin loguear no se puede mostrar en usuarios conectados");
			}
		}
	}

	string mensaje = mensajeDeRed->getComandoClienteSerializado();
	int tamanio = mensaje.length() + 1;
	this->conexionConCliente->getSocket().enviarDatos(mensaje.c_str(), tamanio);
}

void Conexion::enviarChatGlobal(bool tipoDeChat, string unEmisor, string unMensaje) {
	Logger::getInstance()->log(Debug, "El envio de mensaje fue satisfactorio");
	ComandoCliente comando;
	string resultado;

	if (tipoDeChat) {
		comando = ComandoCliente::RECIEVE_GLOBAL_MESSAGES;
		resultado = "RECIEVE_GLOBAL_MESSAGES_OK";
	} else {
		comando = ComandoCliente::RECIEVE_PRIVATE_MESSAGES;
		resultado = "RECIEVE_PRIVATE_MESSAGES_OK";
	}

	MensajeDeRed* mensajeDeRed = new MensajeDeRed(comando);
	mensajeDeRed->agregarParametro(resultado);
	mensajeDeRed->agregarParametro(unEmisor);
	mensajeDeRed->agregarParametro(unMensaje);
	string mensaje = mensajeDeRed->getComandoClienteSerializado();
	int tamanio = mensaje.length() + 1;
	Logger::getInstance()->log(Debug, "Enviando mensaje");
	Logger::getInstance()->log(Debug, mensaje);
	this->conexionConCliente->getSocket().enviarDatos(mensaje.c_str(), tamanio);
}


/* Procesa los mensajes recibidos por los clientes */
void Conexion::procesarDatosRecibidos() {
	auto timeOut = std::chrono::high_resolution_clock::now();
	while (conexionActiva) {
		char* datosRecibidos = this->conexionConCliente->getMensaje();
		if (datosRecibidos != NULL) {
			timeOut = std::chrono::high_resolution_clock::now();
			Logger::getInstance()->log(Debug, datosRecibidos);
			std::string datosRecibidosString(datosRecibidos);
			MensajeDeRed *mensajeDeRed = new MensajeDeRed(datosRecibidosString, Constantes::SERVIDOR);
			Usuario* unUsuario = NULL;
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
				procesarSolicitudPing();
				break;
			case ComandoServidor::SEND_MESSAGE:
				Logger::getInstance()->log(Debug, "Recibio un Send_message");
				procesarSend_Message(mensajeDeRed);
				break;
			case ComandoServidor::RETRIEVE_MESSAGES:
				Logger::getInstance()->log(Debug, "Recibio un Retrieve_message");
				procesarRetrieve_Messages(mensajeDeRed);
				break;
			case ComandoServidor::USUARIOS:
				Logger::getInstance()->log(Debug, "Recibio peticion de Usuarios");
				procesarPeticionListaDeUsuarios();
				break;
			default:
				Logger::getInstance()->log(Debug, datosRecibidos);
			}
			if (datosRecibidos != NULL)
				free(datosRecibidos);
		}

		if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - timeOut).count() > (Constantes::PING_DELAY + 1000 * 3))
			this->cerrarConexion();
	}
}

