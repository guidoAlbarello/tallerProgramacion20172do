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
}

Servidor::~Servidor() {
	delete conexionDelServidor;
}

std::vector<Conexion*> Servidor::getConexionesActivas() {
	return this->conexionesActivas;
}

void Servidor::iniciarServidor() {
	this->leerServerConfig();

	this->conexionDelServidor->iniciarConexion(this->configuracion->getPuerto(), this->configuracion->getMaxClientes());
	this->t_escucharClientes = std::thread(&Servidor::escucharClientes, this);
	this->correrCicloPrincipal();
}

void Servidor::cerrarServidor() {
	this->conexionDelServidor->cerrarConexion();
	this->servidorActivo = false;
}

void Servidor::leerServerConfig() {
	this->configuracion->leerConfiguracion();
}

Usuario Servidor::buscarUsuario(std::string unUsuario) {
	Usuario usuarioDestinatario = Usuario();
	std::vector<Usuario> listaDeUsuarios = this->configuracion->getUsuarios();

	for (int i = 0; i < listaDeUsuarios.size() && usuarioDestinatario.getNombre() == ""; i++) {
		if (unUsuario.compare(listaDeUsuarios[i].getNombre()) == 0)
			usuarioDestinatario = listaDeUsuarios[i];
	}

	return usuarioDestinatario;
}

bool Servidor::usuarioValido(std::string usuarioBuscado, std::string contraseniaBuscada) {
	std::vector<Usuario> listaDeUsuarios = this->configuracion->getUsuarios();
	bool encontrado = false;
	std::transform(usuarioBuscado.begin(), usuarioBuscado.end(), usuarioBuscado.begin(), ::toupper);
	std::transform(contraseniaBuscada.begin(), contraseniaBuscada.end(), contraseniaBuscada.begin(), ::toupper);
	for (int i = 0; i < listaDeUsuarios.size(); i++) {
		std::string nombreActual = listaDeUsuarios[i].getNombre();
		std::string contraseniaActual = listaDeUsuarios[i].getPassword();
		std::transform(nombreActual.begin(), nombreActual.end(), nombreActual.begin(), ::toupper);
		std::transform(contraseniaActual.begin(), contraseniaActual.end(), contraseniaActual.begin(), ::toupper);
		if (usuarioBuscado.compare(nombreActual) == 0 && contraseniaBuscada.compare(contraseniaActual) == 0) {
			encontrado = true;
			break;
		}
	}
	return encontrado;
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
	for (int i = 0; i < conexionesActivas.size(); i++) {
		conexionesActivas[i]->cerrarConexion();
	}
}

void Servidor::cambiarNivelLogeo() {
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
				break;
			case '2':
				Logger::getInstance()->setMode(LogMode::Actividad);
				break;
			case '3':
				Logger::getInstance()->setMode(LogMode::Debug);
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
	if (conexionesActivas.size() == 0) {
		std::cout << "En este momento no hay usuarios conectados a la aplicacion" << std::endl;
	}
	for (int i = 0; i < conexionesActivas.size(); i++) {
		if (conexionesActivas[i]->getUsuario() != NULL) {
			std::cout << "Usuario: " << conexionesActivas[i]->getUsuario()->getNombre() << std::endl;
		}
		else {
			std::cout << "Usuario connectado sin loguear" << std::endl;
		}
	}
	std::cout << std::endl;
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

bool Servidor::validarLogin(MensajeDeRed* mensaje) {
	std::string usuario = mensaje->getParametro(0);
	std::string contrasenia = mensaje->getParametro(1);

	bool usuarioExiste = this->usuarioValido(usuario, contrasenia);
	if (usuarioExiste) {
		return true;
	}
	return false;
}

