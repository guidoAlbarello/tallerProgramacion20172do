#include "Servidor.h"

Servidor* Servidor::instance = 0;

Servidor * Servidor::getInstance() {
	if (!instance) {
		instance = new Servidor();
	}

	return instance;
}

const std::string DEFAULT_SERVER_CONFIG_FILE = "server-config.xml";

Servidor::Servidor() {
	this->conexionDelServidor = new	ManejadorDeConexionServidor();
	this->servidorActivo = true;
}

Servidor::~Servidor() {
	delete conexionDelServidor;
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
	if (this->existeArchivo(DEFAULT_SERVER_CONFIG_FILE)) {
		ParserXml* xmlParser = new ParserXml();
		this->configuracion = xmlParser->readServerConfigFile(DEFAULT_SERVER_CONFIG_FILE);
	}
	else {
		this->configuracion = new ServerConfig();
		this->configuracion->crearArchivoConfiguracion(DEFAULT_SERVER_CONFIG_FILE);
	}
}

bool Servidor::existeArchivo(const std::string& nombreDeArchivo) {
	std::ifstream archivo(nombreDeArchivo.c_str());
	return (bool)archivo;
}

Usuario * Servidor::buscarUsuario(std::string unUsuario) {
	Usuario* usuarioDestinatario = NULL;

	for (int i = 0; i < listaDeUsuarios.size() && usuarioDestinatario == NULL; i++) {
		if (unUsuario.compare(listaDeUsuarios[i]->getNombre()) == 0)
			usuarioDestinatario = listaDeUsuarios[i];
	}

	return usuarioDestinatario;
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
		SOCKET nuevoCliente = this->conexionDelServidor->hayClienteIntentandoConectarse();
		if(nuevoCliente != INVALID_SOCKET) {
			agregarNuevaConexionEntrante(nuevoCliente);
		}
	}
}

void Servidor::agregarNuevaConexionEntrante(SOCKET unCliente) {
	Conexion* nuevaConexion = new Conexion(unCliente);
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
