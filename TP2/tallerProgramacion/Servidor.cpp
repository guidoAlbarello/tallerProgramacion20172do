#include "Conexion.h"
#include "Servidor.h"
#include <algorithm>

Servidor* Servidor::instance = 0;

Servidor* Servidor::getInstance() {
	if (!instance) {
		instance = new Servidor();
	}

	return instance;
}

Servidor::Servidor() {
	this->conexionDelServidor = new	ManejadorDeConexionServidor();
	this->servidorActivo = true;
	this->configuracion = new ServerConfig();
	this->buzonDeChatGlobal = new BuzonDeMensajes();
	this->elJuego = new Juego();

	//Seteando el nombre del log
	Logger::getInstance()->setLogFileName(SERVER_LOG_FILENAME_FORMAT);
}

Servidor::~Servidor() {
	delete conexionDelServidor;
}

std::vector<Conexion*> Servidor::getConexionesActivas() {
	return this->conexionesActivas;
}

void Servidor::recibirMensajeGlobal(string unEmisor, string unMensaje) {
	this->buzonDeChatGlobal->recibirMensaje("", unEmisor, unMensaje);
}

void Servidor::enviarMensajePrivado(string unDestinatario, string unMensaje) {
	bool encontrado = false;
	for (int i = 0; i < this->conexionesActivas.size() && !encontrado; i++) {
		Conexion* unaConexion = this->conexionesActivas[i];
		if (unaConexion->getUsuario()->getNombre().compare(unDestinatario) == 0) {
			unaConexion->enviarChatGlobal(false, unDestinatario, unMensaje);
			encontrado = true;
		}
	}
}

bool Servidor::estaElUsuarioConectado(Usuario * unUsuario) {
	bool usuarioConectado = false;

	for (std::vector<Conexion*>::iterator it = conexionesActivas.begin(); it != conexionesActivas.end() && !usuarioConectado; ++it) {
		Conexion* unaConexion = (Conexion*)*it;
		if (unaConexion->getUsuario() != NULL)
			if (unaConexion->getUsuario()->getNombre().compare(unUsuario->getNombre()) == 0)
				usuarioConectado = true;
	}

	return usuarioConectado;
}

void Servidor::iniciarServidor() {
	try {
		Logger::getInstance()->log(Debug, "Iniciando servidor...");
		this->leerServerConfig();
		if (this->elJuego->iniciarJuego()) {   //pasasrle por param la configuracion del xml
			this->conexionDelServidor->iniciarConexion(this->configuracion->getPuerto(), this->configuracion->getMaxClientes());

			this->t_escucharClientes = std::thread(&Servidor::escucharClientes, this);
			//this->t_enviarChatGlobal = std::thread(&Servidor::enviarChatGlobal, this);
			this->t_updateModel = std::thread(&Servidor::updateModel, this);
			this->correrCicloPrincipal();
		}
	} catch (exception e) {

	}
}

void Servidor::cerrarServidor() {
	Logger::getInstance()->log(Debug, "se realiza el cierre del servidor");
	this->servidorActivo = false;
	try {
		this->conexionDelServidor->cerrarConexion();
		delete this->conexionDelServidor;
		delete this->configuracion;
		delete this->buzonDeChatGlobal;

		if (t_escucharClientes.joinable()) {
			t_escucharClientes.join();
		}

		//if (t_enviarChatGlobal.joinable()) {
		//	t_enviarChatGlobal.join();
		//}

		if (t_updateModel.joinable()) {
			t_updateModel.join();
		}
	} catch (exception e) {
		Logger::getInstance()->log(Error, "Ocurrio un error al cerrar el servidor");
	}

}

void Servidor::verificarConexiones() {
	if (this->conexionesActivas.size() > 0) {
		for (int i = 0; i < this->conexionesActivas.size(); i++) {
			if (!this->conexionesActivas.at(i)->getConexionActiva()) {
				this->conexionesActivas.at(i)->cerrarConexion();
				delete (this->conexionesActivas.at(i));
				this->conexionesActivas.erase(this->conexionesActivas.begin() + i);
			}
		}
	}

}

void Servidor::leerServerConfig() {
	Logger::getInstance()->log(Debug, "se lee la configuracion del servidor");
	this->configuracion->leerConfiguracion();
}

Usuario* Servidor::buscarUsuario(std::string unUsuario) {
	Usuario* usuarioDestinatario = NULL;
	std::vector<Usuario*> listaDeUsuarios = this->configuracion->getUsuarios();

	for (unsigned int i = 0; i < listaDeUsuarios.size() && usuarioDestinatario == NULL; i++) {
		if (unUsuario.compare(listaDeUsuarios[i]->getNombre()) == 0)
			usuarioDestinatario = listaDeUsuarios[i];
	}

	return usuarioDestinatario;
}

Usuario* Servidor::usuarioValido(std::string usuarioBuscado, std::string contraseniaBuscada) {

	std::vector<Usuario*> listaDeUsuarios = this->configuracion->getUsuarios();
	Usuario* unUsuario = NULL;
	std::transform(usuarioBuscado.begin(), usuarioBuscado.end(), usuarioBuscado.begin(), ::toupper);
	std::transform(contraseniaBuscada.begin(), contraseniaBuscada.end(), contraseniaBuscada.begin(), ::toupper);
	for (unsigned int i = 0; i < listaDeUsuarios.size(); i++) {
		std::string nombreActual = listaDeUsuarios[i]->getNombre();
		std::string contraseniaActual = listaDeUsuarios[i]->getPassword();
		std::transform(nombreActual.begin(), nombreActual.end(), nombreActual.begin(), ::toupper);
		std::transform(contraseniaActual.begin(), contraseniaActual.end(), contraseniaActual.begin(), ::toupper);
		if (usuarioBuscado.compare(nombreActual) == 0 && contraseniaBuscada.compare(contraseniaActual) == 0) {
			unUsuario = listaDeUsuarios[i];
			break;
		}
	}
	return unUsuario;
}

void Servidor::correrCicloPrincipal() {
	std::string input;
	while (servidorActivo) {
		mostrarMenuPrincipal();
		std::getline(std::cin, input);
		if (input.length() != 1) {
			std::cout << "Debe ingresar una de las opciones" << std::endl;
		} else {
			char opcionElegida = input[0];
			Logger::getInstance()->log(Actividad, "se ingresa la opcion " + input + "en el menu de servidor");
			switch (opcionElegida) {
			case '1':
				if (this->conexionesActivas.size() != 0) {
					string confirmacionUsuario;
					while (confirmacionUsuario.length() != 1) {
						std::cout << "Todavia hay conexiones activas en el servidor, esta seguro que desea cerrarlas? Para cerrarlas presione la tecla 'y' , sino presione cualquier otra: ";
						std::getline(std::cin, confirmacionUsuario);
					}

					if (confirmacionUsuario.compare("y") == 0)
						cerrarTodasLasConexiones();
				} else {
					cerrarTodasLasConexiones();
				}

				break;
			case '2':
				cambiarNivelLogeo();
				break;
			case '3':
				mostrarUsuariosConectados();
				break;
			case '4':
				cerrarTodasLasConexiones();
				cerrarServidor();
				break;
			default:
				break;
			}
		}
	}
}

void Servidor::escucharClientes() {
	while (this->servidorActivo) {
		SOCKET nuevoCliente = this->conexionDelServidor->hayClienteIntentandoConectarse(this->conexionesActivas.size(), this->configuracion->getMaxClientes());
		if (nuevoCliente != INVALID_SOCKET) {
			agregarNuevaConexionEntrante(nuevoCliente);
		}
	}
}

void Servidor::agregarNuevaConexionEntrante(SOCKET unCliente) {
	Conexion* nuevaConexion = new Conexion(unCliente, this);
	conexionesActivas.push_back(nuevaConexion);
}

void Servidor::cerrarTodasLasConexiones() {
	Logger::getInstance()->log(Debug, "Cerrando todas las conexiones del servidor...");
	for (unsigned int i = 0; i < conexionesActivas.size(); i++) {
		Conexion* unaConexion = conexionesActivas[i];
		unaConexion->cerrarConexion();
		delete unaConexion;
	}

	conexionesActivas.erase(conexionesActivas.begin(), conexionesActivas.end());
}

void Servidor::cambiarNivelLogeo() {
	Logger::getInstance()->log(Debug, "Cambiando nivel de logueo...");
	std::string input;
	while (input.length() != 1) {
		mostrarMenuModosLogueo();
		std::getline(std::cin, input);
		if (input.length() != 1) {
			std::cout << "Debe ingresar una de las opciones" << std::endl;
		} else {
			char opcionElegida = input[0];
			Logger::getInstance()->log(Actividad, "se ingresa la opcion " + input + " en el menu de cambio de nivel de logue0");
			switch (opcionElegida) {
			case '1':
				Logger::getInstance()->setMode(LogMode::Error);
				Logger::getInstance()->log(Debug, "Nuevo modo de logueo = ERROR");
				break;
			case '2':
				Logger::getInstance()->setMode(LogMode::Actividad);
				Logger::getInstance()->log(Debug, "Nuevo modo de logueo = ACTIVIDAD");
				break;
			case '3':
				Logger::getInstance()->setMode(LogMode::Debug);
				Logger::getInstance()->log(Debug, "Nuevo modo de logueo = DEBUG");
				break;
			case '4':
				break;
			default:
				break;
			}
		}
	}
	correrCicloPrincipal();
}

void Servidor::mostrarUsuariosConectados() {
	mostrarMenuUsuariosConectados();
	Logger::getInstance()->log(Debug, "Mostrando usuarios conectados");
	if (conexionesActivas.size() == 0) {
		std::cout << "En este momento no hay usuarios conectados a la aplicacion" << std::endl;
		Logger::getInstance()->log(Debug, "En este momento no hay usuarios conectados a la aplicacion");
	} else {
		std::vector<std::string> usuariosConectados;
		bool hayUsuariosConConexionActiva = false;
		for (unsigned int i = 0; i < conexionesActivas.size(); i++) {
			if (conexionesActivas[i]->getConexionActiva()) {
				hayUsuariosConConexionActiva = true;
				if (conexionesActivas[i]->getUsuario() != NULL) {
					usuariosConectados.push_back(conexionesActivas[i]->getUsuario()->getNombre());
				} else {
					usuariosConectados.push_back("Usuario conectado sin loguear");
				}
			}
		}
		if (hayUsuariosConConexionActiva) {
			Logger::getInstance()->log(Debug, "Usuarios conectados:");
			for (int i = 0; i < usuariosConectados.size(); i++) {
				std::cout << usuariosConectados[i] << std::endl;
				Logger::getInstance()->log(Debug, usuariosConectados[i]);
			}
		} else {
			std::cout << "En este momento no hay usuarios conectados a la aplicacion" << std::endl;
			Logger::getInstance()->log(Debug, "En este momento no hay usuarios conectados a la aplicacion");
		}
		std::cout << std::endl;
	}
}

void Servidor::mostrarMenuPrincipal() {
	cout << "|----------------------------|" << std::endl;
	cout << "|        Menu servidor       |" << std::endl;
	cout << "|----------------------------|" << std::endl;
	std::cout << "1.Close-server" << std::endl;
	std::cout << "2.Change-log-level" << std::endl;
	std::cout << "3.Show-connected-users" << std::endl;
	std::cout << "4.Exit" << std::endl;
}

void Servidor::mostrarMenuModosLogueo() {
	cout << "|----------------------------|" << std::endl;
	cout << "|       Modos de logeo       |" << std::endl;
	cout << "|----------------------------|" << std::endl;
	std::cout << "Modo actual: ";
	LogMode modoLogueoActual = Logger::getInstance()->getMode();
	switch (modoLogueoActual) {
	case LogMode::Error:
		std::cout << "Error" << std::endl;
		break;
	case LogMode::Actividad:
		std::cout << "Actividad" << std::endl;
		break;
	case LogMode::Debug:
		std::cout << "Debug" << std::endl;
		break;
	}
	std::cout << std::endl;
	std::cout << "1.Modo Error" << std::endl;
	std::cout << "2.Modo Actividad" << std::endl;
	std::cout << "3.Modo Debug" << std::endl;
	std::cout << "4.Volver" << std::endl;
}

void Servidor::mostrarMenuUsuariosConectados() {
	std::cout << "|----------------------------|" << std::endl;
	std::cout << "|     Usuarios conectados    |" << std::endl;
	std::cout << "|----------------------------|" << std::endl;
}

Usuario* Servidor::validarLogin(MensajeDeRed* mensaje, string &mensajeResultado) {
	std::string usuario = mensaje->getParametro(0);
	std::string contrasenia = mensaje->getParametro(1);

	Usuario* unUsuario = this->usuarioValido(usuario, contrasenia);
	if (unUsuario == NULL) {
		mensajeResultado = "Login Invalido. EL usuario o la password son incorrectos.";
		return unUsuario;
	}
	if (this->estaElUsuarioConectado(unUsuario)) {
		mensajeResultado = "Login invalido. El usuario ya se encuentra conectado.";
		unUsuario = NULL;
	} else {
		mensajeResultado = "El login fue satisfactorio";
		if(!unUsuario->tieneJugadorAsignado())
			unUsuario->setJugador(this->elJuego->agregarJugador());  //cuando se logea por segunda vez, va a explotar
	}
	return unUsuario;
}

void Servidor::enviarChatGlobal() {
	while (servidorActivo) {
		this->verificarConexiones();
		int tamanioBuzon = buzonDeChatGlobal->getTamanio();
		if (tamanioBuzon != 0) {
			int i = 0;
			for (i = 0; i < tamanioBuzon; i++) {
				Mensaje* unMensaje = this->buzonDeChatGlobal->verMensaje(i);
				for (std::vector<Conexion*>::iterator it = conexionesActivas.begin(); it != conexionesActivas.end(); ++it) {
					Conexion* unaConexion = (Conexion*)*it;
					if (unaConexion->getUsuario() != NULL)
						unaConexion->enviarChatGlobal(true, unMensaje->getEmisor(), unMensaje->getMensaje());
				}

			}
			if (i > 0)
				this->buzonDeChatGlobal->eliminarMensajes(i);
		}
	}
}

void Servidor::updateModel() {
	while (servidorActivo) {
		for (std::vector<Conexion*>::iterator it = conexionesActivas.begin(); it != conexionesActivas.end(); ++it) {
			Conexion* unaConexion = (Conexion*)*it;
			if (unaConexion->getUsuario() != NULL) {

				// TODO: ver si es la clase Juego la que manda esto o el Servidor
				Constantes::EstadoJugador estadoJugador1;
				estadoJugador1.posX = 10.0;
				estadoJugador1.posY = 20.0;
				estadoJugador1.km = 22.5;
				estadoJugador1.usuario = "tomas";
				EstadoAuto estadoAuto1 = EstadoAuto::DERECHO;
				estadoJugador1.estadoAuto = estadoAuto1;

				Constantes::EstadoJugador estadoJugador2;
				estadoJugador2.posX = 50.0;
				estadoJugador2.posY = 50.0;
				estadoJugador2.km = 22.5;
				estadoJugador2.usuario = "pepe";
				EstadoAuto estadoAuto2 = EstadoAuto::DOBLANDO_DER;
				estadoJugador2.estadoAuto = estadoAuto2;

				Constantes::EstadoJugador estadoJugador3;
				estadoJugador3.posX = 50.0;
				estadoJugador3.posY = 50.0;
				estadoJugador3.km = 22.5;
				estadoJugador3.usuario = "pepe";
				EstadoAuto estadoAuto3 = EstadoAuto::DOBLANDO_DER;
				estadoJugador3.estadoAuto = estadoAuto3;

				Constantes::EstadoJugador estadoJugador4;
				estadoJugador4.posX = 50.0;
				estadoJugador4.posY = 50.0;
				estadoJugador4.km = 22.5;
				estadoJugador4.usuario = "pepe";
				EstadoAuto estadoAuto4 = EstadoAuto::DOBLANDO_DER;
				estadoJugador4.estadoAuto = estadoAuto4;

				Constantes::EstadoJuego estadoJuego;
				estadoJuego.estados[0] = estadoJugador1;
				estadoJuego.estados[1] = estadoJugador2;
				estadoJuego.estados[2] = estadoJugador3;
				estadoJuego.estados[3] = estadoJugador4;

				unaConexion->enviarUpdate(estadoJuego);
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(Constantes::UPDATE_MODEL_DELAY));
	}

}

