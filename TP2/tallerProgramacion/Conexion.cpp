
#include "Conexion.h"


Conexion::Conexion(SOCKET unSocket, Servidor* servidor) {
	this->servidor = servidor;
	this->conexionConCliente = new ManejadorDeConexionConexion(unSocket);
	this->conexionConCliente->iniciarConexion();
	this->conexionActiva = true;
	this->conexionViva = true;
	this->usuarioConectado = NULL;
	this->t_procesarDatosRecibidos = std::thread(&Conexion::procesarDatosRecibidos, this);
}

Conexion::~Conexion() {

}

void Conexion::cerrarConexion() {
	if (!cerrando) {
		cerrando = true;
		this->conexionActiva = false;
		this->conexionViva = false;
		this->conexionInicializada = false;
		if (this->getUsuario() != NULL) {
			if (this->getUsuario()->getJugador() != NULL) {
				this->getUsuario()->getJugador()->setEstadoConexion(false);
				this->getUsuario()->getJugador()->recibirEntrada(0, false);//ponemos todas las entradas en false para q frene. 
				this->getUsuario()->getJugador()->recibirEntrada(1, false);
				this->getUsuario()->getJugador()->recibirEntrada(2, false);
			}
		}

		try {
			if (t_procesarDatosRecibidos.joinable()) {
				t_procesarDatosRecibidos.join();
			}

			if (this->conexionConCliente != NULL) {
				this->conexionConCliente->cerrarConexion();
				delete this->conexionConCliente;
			}
		}

		catch (exception e) {
			Logger::getInstance()->log(Error, "Ocurrio un error al cerrar la conexion");
		}
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

void Conexion::procesarInput(bool* entrada) {
	if (this->usuarioConectado != NULL) {
		if (this->usuarioConectado->getJugador() != NULL) {
			for (int i = 0; i < Constantes::CANT_TECLAS; i++)
				this->usuarioConectado->getJugador()->recibirEntrada(i, entrada[i]);
		}
	}
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
			ComandoCliente comando = ComandoCliente::RESULTADO_SEND_MESSAGE;
			MensajeDeRed* mensajeDeRed = new MensajeDeRed(comando);
			mensajeDeRed->agregarParametro("SEND_MESSAGE_OK"); // ResultCode
			if (this->servidor->estaElUsuarioConectado(usuarioDestinatario)) {
				this->getUsuario()->enviarMensaje(usuarioDestinatario, unMensajeDeRed->getParametro(1));
				mensajeDeRed->agregarParametro("El envio del mensaje fue satisfactorio");
			} else {
				mensajeDeRed->agregarParametro("No se pudo enviar el mensaje. El destinatario no se encuentra conectado.");
			}
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

void Conexion::enviarUpdate(EstadoModeloJuego* estado) {
	Logger::getInstance()->log(Debug, "Enviando update");

	int tamanio = sizeof(EstadoModeloJuego) + 12 + 1;  //+ tamaño "update_model" + caracter separador
	char* data = new char[tamanio];
	std::string strComando = "UPDATE_MODEL";
	strComando.append(&Constantes::separador);
	const char* comando = strComando.c_str();
	memcpy(data, comando, 13);
	memcpy(data + 13, estado, sizeof(EstadoModeloJuego));

	this->conexionConCliente->getSocket().enviarDatos(data, tamanio);

	if (data != NULL)
		free(data);
}

void Conexion::inicializarClienteJuego(EstadoInicialJuego * estado, Usuario* unUsuario) {
	Logger::getInstance()->log(Debug, "Enviando init");
	
	if (unUsuario == NULL)
		unUsuario = this->usuarioConectado;

	estado->idJugador = unUsuario->getJugador()->getId();
	int tamanio = sizeof(EstadoInicialJuego) + 4 + 1;  //+ tamaño "update_model" + caracter separador
	char* data = new char[tamanio];
	std::string strComando = "INIT";
	strComando.append(&Constantes::separador);
	const char* comando = strComando.c_str();
	memcpy(data, comando, 5);
	memcpy(data + 5, estado, sizeof(EstadoInicialJuego));
	
	this->conexionConCliente->getSocket().enviarDatos(data, tamanio);

	if (data != NULL)
		free(data);
}


/* Procesa los mensajes recibidos por los clientes */
void Conexion::procesarDatosRecibidos() {
	auto timeOut = std::chrono::high_resolution_clock::now();
	while (conexionActiva) {
		char* datosRecibidos = this->conexionConCliente->getMensaje();

		if (datosRecibidos != NULL) {
			string mensajeResultado;
			timeOut = std::chrono::high_resolution_clock::now();
			Logger::getInstance()->log(Debug, datosRecibidos);
			std::string datosRecibidosString(datosRecibidos);
			MensajeDeRed *mensajeDeRed = new MensajeDeRed(datosRecibidosString, Constantes::SERVIDOR);
			Usuario* unUsuario = NULL;
			bool enviarEstadoInicial = false;
			bool entrada[Constantes::CANT_TECLAS];
			switch (mensajeDeRed->getComandoServidor()) {
			case ComandoServidor::LOG:
				Logger::getInstance()->log(Debug, "Recibio un LOG");
				// Envia respuesta con el resultado del login
				unUsuario = this->servidor->validarLogin(mensajeDeRed, mensajeResultado, enviarEstadoInicial);
				if (unUsuario != NULL) {

					Logger::getInstance()->log(Debug, "El login fue satisfactorio");
					if (!enviarEstadoInicial) {
						ComandoCliente comando = ComandoCliente::RESULTADO_LOGIN;
						MensajeDeRed* mensajeDeRed = new MensajeDeRed(comando);
						mensajeDeRed->agregarParametro("LOGIN_OK"); // ResultCode
						mensajeDeRed->agregarParametro("0");
						mensajeDeRed->agregarParametro(to_string(unUsuario->getJugador()->getId()));
						mensajeDeRed->agregarParametro(mensajeResultado);
						string mensaje = mensajeDeRed->getComandoClienteSerializado();
						int tamanio = mensaje.length() + 1;
						Logger::getInstance()->log(Debug, "Enviando mensaje");
						Logger::getInstance()->log(Debug, mensaje);
					
						this->conexionConCliente->getSocket().enviarDatos(mensaje.c_str(), tamanio);
					
					} else {
						EstadoInicialJuego* estadoInicial = this->servidor->getJuego()->getEstadoJuegoInicial();
						estadoInicial->tamanio = this->servidor->getConfiguracion()->getMaxClientes();

						estadoInicial->idJugador = unUsuario->getJugador()->getId();
						int tamanio = sizeof(EstadoInicialJuego) + 27;
						char* data = new char[tamanio];
						std::string strComando = "RESULTADO_LOGIN";
						strComando.append(&Constantes::separador).append("LOGIN_OK").append(&Constantes::separador).append("1").append(&Constantes::separador);
						const char* comando = strComando.c_str();
						memcpy(data, comando, 27);
						memcpy(data + 27, estadoInicial, sizeof(EstadoInicialJuego));
						
						this->conexionConCliente->getSocket().enviarDatos(data, tamanio);
					

						delete estadoInicial;
						if (data != NULL)
							free(data);
					}
					this->usuarioConectado = unUsuario;
				} else {
					Logger::getInstance()->log(Debug, "Login invalido");
					ComandoCliente comando = ComandoCliente::RESULTADO_LOGIN;
					MensajeDeRed* mensajeDeRed = new MensajeDeRed(comando);
					mensajeDeRed->agregarParametro("LOGIN_NOK"); // ResultCode
					mensajeDeRed->agregarParametro(mensajeResultado);
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
			case ComandoServidor::INPUT:
				Logger::getInstance()->log(Debug, "Recibio Input");
				this->conexionInicializada = true;
				memcpy(entrada, datosRecibidos + 6, sizeof(bool) * Constantes::CANT_TECLAS);
				procesarInput(entrada);
				break;
			default:
				Logger::getInstance()->log(Debug, datosRecibidos);
			}
			if (datosRecibidos != NULL) {
				free(datosRecibidos);
			}
			timeOut = std::chrono::high_resolution_clock::now();
		}

		double tiempoTardado = std::chrono::duration_cast<std::chrono::duration<double, std::ratio<1>>>(std::chrono::high_resolution_clock::now() - timeOut).count() * 1000;
		if (this->conexionInicializada && tiempoTardado > (Constantes::PING_DELAY) + Constantes::TOLERANCIA_PING) {
			this->conexionActiva = false;
			this->conexionViva = false;
			this->conexionInicializada = false;
			//this->cerrarConexion();
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(Constantes::UPDATE_MODEL_DELAY));//esdto se podria cambiar x un while hasta q no pase el intervalo de tiempo, y mientras q no pase aprovechar el tiempo para hacer clean ups  
	}
}

