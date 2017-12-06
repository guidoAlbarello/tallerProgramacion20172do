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

bool Servidor::usuarioDentroDeJuego(Usuario * unUsuario) {
	bool resultado = false;

	if (yaEnvioEstado) {
		std::vector<Usuario*> listaDeUsuarios = this->configuracion->getUsuarios();

		for (unsigned int i = 0; i < listaDeUsuarios.size() && resultado == false; i++) {
			if (listaDeUsuarios[i]->getJugador() != NULL && listaDeUsuarios[i]->getNombre().compare(unUsuario->getNombre()) == 0)
				resultado = true;
		}
	}
	else {
		// Para el primer logueo
		resultado = true;
	}

	return resultado;
}

Servidor::Servidor() {
	this->conexionDelServidor = new	ManejadorDeConexionServidor();
	this->servidorActivo = true;
	this->configuracion = new ServerConfig();
	this->buzonDeChatGlobal = new BuzonDeMensajes();
	this->elJuego = new Juego();
	this->mapa[0] = new Mapa(1);
	this->mapa[1] = new Mapa(2);
	this->mapa[2] = new Mapa(3);
	this->elJuego->setMapa(0, this->mapa[0]);
	this->elJuego->setMapa(1, this->mapa[1]);
	this->elJuego->setMapa(2, this->mapa[2]);

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
		if (this->elJuego->iniciarJuego(configuracion->getMaxClientes())) {  
			this->conexionDelServidor->iniciarConexion(this->configuracion->getPuerto(), this->configuracion->getMaxClientes());

			this->t_escucharClientes = std::thread(&Servidor::escucharClientes, this);
			this->t_updateModel = std::thread(&Servidor::updateModel, this);
			this->correrCicloPrincipal();
		}
	} catch (exception e) {
		Logger::getInstance()->log(Error, "Ocurrio un error al iniciar el servidor");
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
		SOCKET nuevoCliente = this->conexionDelServidor->hayClienteIntentandoConectarse(this->conexionesActivas, this->configuracion->getMaxClientes());
		if (nuevoCliente != INVALID_SOCKET) {
			agregarNuevaConexionEntrante(nuevoCliente);
		}
	}
}

void Servidor::agregarNuevaConexionEntrante(SOCKET unCliente) {
	m_agregarConexion.lock();
	Conexion* nuevaConexion = new Conexion(unCliente, this);
	conexionesActivas.push_back(nuevaConexion);
	m_agregarConexion.unlock();
}

void Servidor::cerrarTodasLasConexiones() {
	Logger::getInstance()->log(Debug, "Cerrando todas las conexiones del servidor...");
	m_agregarConexion.lock();
	cerrandoConexiones = true;
	for (unsigned int i = 0; i < conexionesActivas.size(); i++) {
		Conexion* unaConexion = conexionesActivas[i];
		unaConexion->cerrarConexion();
		delete unaConexion;
	}

	conexionesActivas.erase(conexionesActivas.begin(), conexionesActivas.end());
	cerrandoConexiones = false;
	m_agregarConexion.unlock();
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

Usuario* Servidor::validarLogin(std::string usuario, std::string contrasenia, string &mensajeResultado, bool& enviarEstadoInicial) {
	
	Usuario* unUsuario = this->usuarioValido(usuario, contrasenia);
	if (unUsuario == NULL) {
		mensajeResultado = "Login Invalido. EL usuario o la password son incorrectos.";
		return unUsuario;
	}
	if (this->estaElUsuarioConectado(unUsuario)) {
		mensajeResultado = "Login invalido. El usuario ya se encuentra conectado.";
		unUsuario = NULL;
	} else if (!this->usuarioDentroDeJuego(unUsuario)) {
		mensajeResultado = "Login invalido. El jugador no se encuentra en el juego.";
		unUsuario = NULL;
	} else {
		mensajeResultado = "El login fue satisfactorio";
		if (!unUsuario->tieneJugadorAsignado()) {
			unUsuario->setJugador(this->elJuego->agregarJugador(unUsuario->getNombre()));
		} else {
			enviarEstadoInicial = true;
		}
		unUsuario->getJugador()->setEstadoConexion(true);  
	}
	return unUsuario;
}

void Servidor::updateModel() {
	yaEnvioEstado = false;
	while (servidorActivo) {

		if(!cerrandoConexiones)
			this->verificarConexiones();
		if (elJuego->getJugadores().size() == this->configuracion->getMaxClientes()) {
			if (yaEnvioEstado) {									
				if (!elJuego->gameOver()) {
					if (!this->elJuego->terminoNivel()) {
						EstadoModeloJuego* nuevoEstado = this->elJuego->getEstadoJuego();
						for (std::vector<Conexion*>::iterator it = conexionesActivas.begin(); it != conexionesActivas.end(); ++it) {
							Conexion* unaConexion = (Conexion*)*it;
							if (unaConexion->getUsuario() != NULL && unaConexion->getConexionActiva() && unaConexion->getConexionInicializada()) {
								unaConexion->enviarUpdate(nuevoEstado);
							}
						}
						this->elJuego->liberarModeloEstado(nuevoEstado);
					} else {
						//mandar resumen de puntos para la pantalla?
						for (std::vector<Conexion*>::iterator it = conexionesActivas.begin(); it != conexionesActivas.end(); ++it) {
							Conexion* unaConexion = (Conexion*)*it;
							if (unaConexion->getUsuario() != NULL && unaConexion->getConexionActiva() && unaConexion->getConexionInicializada()) {
								unaConexion->enviarPantallaTransicion();
							}
						}

						std::this_thread::sleep_for(std::chrono::milliseconds(1000 * Constantes::TIEMPO_PANTALLA_TRANSICION));
						this->elJuego->inicializarNivel();
					}
				} else {
					std::this_thread::sleep_for(std::chrono::milliseconds(Constantes::PING_DELAY));
					for (std::vector<Conexion*>::iterator it = conexionesActivas.begin(); it != conexionesActivas.end(); ++it) {
						Conexion* unaConexion = (Conexion*)*it;
						if (unaConexion->getUsuario() != NULL && unaConexion->getConexionActiva() && unaConexion->getConexionInicializada()) {
							unaConexion->enviarGameOver();
						}
					}
					this->servidorActivo = false;
				}
			} else {
				EstadoInicialJuego* estadoInicial = this->elJuego->getEstadoJuegoInicial();
				estadoInicial->tamanio = this->configuracion->getMaxClientes();
				for (std::vector<Conexion*>::iterator it = conexionesActivas.begin(); it != conexionesActivas.end(); ++it) {
					Conexion* unaConexion = (Conexion*)*it;
					if (unaConexion->getUsuario() != NULL && unaConexion->getConexionActiva()) {
						unaConexion->procesarPeticionListaDeUsuarios(this->elJuego->getJugadores());
						unaConexion->inicializarClienteJuego(estadoInicial, NULL);  //    !!!!!!!!!!!!!!!!!!!!esto va a haber q mandarlo cuando se reconecte el cliente tmb. aca solo se va a mandar la primera vez !!!!!!!
					}
				}
				if(estadoInicial != NULL)
					delete estadoInicial;
			}
			yaEnvioEstado = true;	
		} 

		m_agregarConexion.lock();
		for (std::vector<Conexion*>::iterator it = conexionesActivas.begin(); it != conexionesActivas.end(); ++it) {
			Conexion* unaConexion = (Conexion*)*it;
			if (unaConexion->getConexionActiva() && unaConexion->getEnviarPing() && !unaConexion->getProcesarLogin()) {
				unaConexion->procesarSolicitudPing();
			}
			if (unaConexion->getConexionActiva() && unaConexion->getProcesarLogin()) {
				unaConexion->procesarSolicitudLogin();
			}
		}
		m_agregarConexion.unlock();
		std::this_thread::sleep_for(std::chrono::milliseconds(Constantes::UPDATE_MODEL_DELAY));//esdto se podria cambiar x un while hasta q no pase el intervalo de tiempo, y mientras q no pase aprovechar el tiempo para hacer clean ups  
	}

}

