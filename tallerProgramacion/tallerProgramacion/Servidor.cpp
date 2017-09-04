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
	// TODO: definir si el servidor es el que tiene el parser o el main
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
	std::vector<Usuario *> listaDeUsuarios = this->configuracion->getUsuarios();

	for (int i = 0; i < listaDeUsuarios.size() && usuarioDestinatario == NULL; i++) {
		if (unUsuario.compare(listaDeUsuarios[i]->getNombre()) == 0)
			usuarioDestinatario = listaDeUsuarios[i];
	}

	return usuarioDestinatario;
}

void Servidor::correrCicloPrincipal() {
	while (servidorActivo) {
		mostrarMenu();
		char entradaUsuario;
		std::cin >> entradaUsuario;
		switch (entradaUsuario) {
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
		case '5':
			printf("CONFIG: %d %s. USERS: %zd\n", this->configuracion->getMaxClientes(), this->configuracion->getPuerto().c_str(), this->configuracion->getUsuarios().size());
			break;
		default:
			break;
		}
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
	std::cout << "-------------------------------" << std::endl;
	std::cout << "Modos de logeo" << std::endl;
	std::cout << "-------------------------------" << std::endl;
	std::cout << "1.Modo Error" << std::endl;
	std::cout << "2.Modo Actividad" << std::endl;
	std::cout << "3.Modo Debug" << std::endl;
	char entradaUsuario;
	std::cin >> entradaUsuario;
	switch (entradaUsuario) {
	case '1':
		Logger::getInstance()->setMode(LogMode::Error);
	case '2':
		Logger::getInstance()->setMode(LogMode::Actividad);
	case '3':
		Logger::getInstance()->setMode(LogMode::Debug);
	default:
		break;
	}
}

void Servidor::mostrarUsuariosConectados() {
	std::cout << "-------------------------------" << std::endl;
	std::cout << "Usuarios conectados" << std::endl;
	std::cout << "-------------------------------" << std::endl;

	for (int i = 0; i < conexionesActivas.size(); i++) {
		if (conexionesActivas[i]->getUsuario() != NULL) {
			std::cout << "Usuario: " << conexionesActivas[i]->getUsuario()->getNombre() << std::endl;
		}
		else {
			std::cout << "Usuario connectado, pero sin loggear" << std::endl;
		}
	}
}

void Servidor::mostrarMenu() {
	std::cout << "-------------------------------" << std::endl;
	std::cout << "Menu" << std::endl;
	std::cout << "-------------------------------" << std::endl;
	std::cout << "1.Close-server" << std::endl;
	std::cout << "2.Change-log-level" << std::endl;
	std::cout << "3.Show-connected-users" << std::endl;
	std::cout << "4.Exit" << std::endl;
}