#include "Cliente.h"

Cliente::Cliente(){
	this->conexionDelCliente = new	ManejadorDeConexionCliente();
	this->clienteActivo = true;
}

Cliente::~Cliente() {
	delete this->conexionDelCliente;
}

void Cliente::iniciarCliente(std::string ipServidor, std::string puertoServidor) {
	ParserXml xmlParser;
    clientConfig = xmlParser.readClientConfigFile(DEFAULT_USER_CONFIG_FILE);

	this->conexionDelCliente->iniciarConexion(clientConfig->getIP(), clientConfig->getPuerto());
	this->t_procesarDatosRecibidos = std::thread(&Cliente::procesarDatosRecibidos, this);
	this->correrCicloPrincipal();
}

void Cliente::correrCicloPrincipal() {
}

void Cliente::cerrarCliente() {
	this->conexionDelCliente->cerrarConexion();
	this->clienteActivo = false;
}

bool Cliente::existeArchivo(const std::string& nombreDeArchivo) {
	std::ifstream archivo(nombreDeArchivo.c_str());
	return (bool)archivo;
}

void Cliente::leerClientConfig() {
	if (this->existeArchivo(DEFAULT_CLIENT_CONFIG_FILE)) {
		ParserXml* xmlParser = new ParserXml();
		this->configuracion = xmlParser->readClientConfigFile(DEFAULT_CLIENT_CONFIG_FILE);
	} else {
		this->configuracion = new ClientConfig();
		this->configuracion->crearArchivoConfiguracion(DEFAULT_CLIENT_CONFIG_FILE);
	}
}

void Cliente::procesarDatosRecibidos() {
}


//tal vez mas adelante haya que cambiar como inicializamos cuando el cliente se pone activo