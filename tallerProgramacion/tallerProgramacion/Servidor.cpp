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

void Servidor::iniciarServidor() {
	Logger::getInstance()->log(Debug, "Iniciando servidor...");
	this->leerServerConfig();
	this->conexionDelServidor->iniciarConexion(this->configuracion->getPuerto(), this->configuracion->getMaxClientes());
	this->t_escucharClientes = std::thread(&Servidor::escucharClientes, this);
	this->t_enviarChatGlobal = std::thread(&Servidor::enviarChatGlobal, this);
	this->correrCicloPrincipal();
}

void Servidor::cerrarServidor() {
	
	this->servidorActivo = false;

	if (t_escucharClientes.joinable()) {
		t_escucharClientes.join();
	}

	this->conexionDelServidor->cerrarConexion();
}

void Servidor::leerServerConfig() {
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
		while (input.length() != 1) {
			mostrarMenuPrincipal();
			std::getline(std::cin, input);
			if (input.length() != 1) {
				std::cout << "Debe ingresar una de las opciones" << std::endl;
			}
			else {
				char opcionElegida = input[0];
				switch (opcionElegida) {
				case '1':
					cerrarTodasLasConexiones();
					break;
				case '2':
					cambiarNivelLogeo();
					break;
				case '3':
					mostrarUsuariosConectados();
					break;
				case '4':
					servidorActivo = false;
					cerrarTodasLasConexiones();
					break;
				default:
					break;
				}
			}
		}
		correrCicloPrincipal();
	}
}

void Servidor::escucharClientes() {
	while (this->servidorActivo) {
		SOCKET nuevoCliente = this->conexionDelServidor->hayClienteIntentandoConectarse(this->getConexionesActivas().size(), this->configuracion->getMaxClientes());
		if(nuevoCliente != INVALID_SOCKET) {
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
		conexionesActivas[i]->cerrarConexion();
	}
}

void Servidor::cambiarNivelLogeo() {
	Logger::getInstance()->log(Debug, "Cambiando nivel de logueo...");
	std::string input;
	while (input.length() != 1) {
		mostrarMenuModosLogueo();
		std::getline(std::cin, input);
		if (input.length() != 1) {
			std::cout << "Debe ingresar una de las opciones" << std::endl;
		}
		else {
			char opcionElegida = input[0];
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
	}
	else {
		Logger::getInstance()->log(Debug, "Usuarios conectados:");
		for (unsigned int i = 0; i < conexionesActivas.size(); i++) {
			if (conexionesActivas[i]->getUsuario() != NULL) {
				string unUsuario = "Usuario: " + conexionesActivas[i]->getUsuario()->getNombre();
				std::cout << unUsuario << std::endl;
				Logger::getInstance()->log(Debug, unUsuario);
			}
			else {
				std::cout << "Usuario conectado sin loguear" << std::endl;
				Logger::getInstance()->log(Debug, "Usuario conectado sin loguear");
			}
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

Usuario* Servidor::validarLogin(MensajeDeRed* mensaje) {
	std::string usuario = mensaje->getParametro(0);
	std::string contrasenia = mensaje->getParametro(1);

	Usuario* unUsuario = this->usuarioValido(usuario, contrasenia);
	return unUsuario;
}

void Servidor::enviarChatGlobal() {
	while (servidorActivo) {
		if (buzonDeChatGlobal->getTamanio() != 0) {
			for (int i = 0; i < this->buzonDeChatGlobal->getTamanio(); i++) {
				Mensaje* unMensaje = this->buzonDeChatGlobal->verMensaje(i);
				for (std::vector<Conexion*>::iterator it = conexionesActivas.begin(); it != conexionesActivas.end(); ++it) {
					Conexion* unaConexion = (Conexion*)*it;
					unaConexion->enviarChatGlobal(unMensaje->getEmisor(), unMensaje->getMensaje());
				}
			}
			this->buzonDeChatGlobal->eliminarMensajes(this->buzonDeChatGlobal->getTamanio());
		}
	}
}
