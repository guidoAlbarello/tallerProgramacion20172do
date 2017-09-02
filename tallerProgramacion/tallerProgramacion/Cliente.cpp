#include "Cliente.h"


Cliente* Cliente::instance = 0;

Cliente * Cliente::getInstance() {
	if (!instance) {
		instance = new Cliente();
	}

	return instance;
}

Cliente::Cliente(){
	this->conexionDelCliente = new	ManejadorDeConexionCliente();
	this->clienteActivo = true;
}

Cliente::~Cliente() {
	delete this->conexionDelCliente;
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


void Cliente::iniciarCliente() {
	/*ParserXml xmlParser;
	clientConfig = xmlParser.readClientConfigFile(DEFAULT_USER_CONFIG_FILE);*/
	this->leerClientConfig();

	correrCicloPrincipal();
}

void Cliente::correrCicloPrincipal() {
	while (clienteActivo) {
		mostrarMenu();
		char entradaUsuario;
		std::cin >> entradaUsuario;
		switch (entradaUsuario) {
		case '1':
			conectarseAlServidor();
			break;
		case '2':
			desconectarseDelServidor();
		case '3':
			clienteActivo = false;
			break;
		case '4':
			hacerTestDeEstres();
			break;
		case '5':
			revisarBuzon();
			break;
		case '6':
			logearseAlServidor();
			break;
		case '7':
			enviarMensajeAlChat();
			break;
		case '8':
			enviarMensajePrivado();
			break;
		case '9':
			printf("CONFIG: %s %s %s", this->configuracion->getIP().c_str(), this->configuracion->getPuerto().c_str(), this->configuracion->getPath().c_str());
			break;
		default:
			break;
		}
	}
}

void Cliente::mostrarMenu() {
	std::cout << "Menu" << std::endl;
	std::cout << "1.Conectar" << std::endl;
	std::cout << "2.Desconectar" << std::endl;
	std::cout << "3.Salir" << std::endl;
	std::cout << "4.Login" << std::endl;
	std::cout << "5.StressTest" << std::endl;
	std::cout << "6.Revisar Buzon" << std::endl;
	std::cout << "7.Mensaje Chat" << std::endl;
	std::cout << "8.Mensaje Privado" << std::endl;
}

void Cliente::conectarseAlServidor() {
	this->conexionDelCliente->iniciarConexion(clientConfig->getIP(), clientConfig->getPuerto().c_str());
	this->t_procesarDatosRecibidos = std::thread(&Cliente::procesarDatosRecibidos, this);
}

void Cliente::desconectarseDelServidor() {
	this->conexionDelCliente->cerrarConexion();
}

void Cliente::hacerTestDeEstres() {

}

void Cliente::revisarBuzon() {
	//this->conexionDelCliente->ejecutarComando(Comando::RETRIEVE_MESSAGES) 
}

void Cliente::logearseAlServidor() {
	//this->conexionDelCliente->ejecutarComando(Comando::LOG, user, pass)
	//char* datosAEnviar = String(Comando::LOG + cliente->usuario + cliente->pass).c_str();
	//int tamanio = String(Comando::LOG + cliente->usuario + cliente->pass).size();
	////this->conexionDelCliente->pasarDatosAEnviar(datosAEnviar, tamanio);

	//mutex en el buffer de manejar conexion .- cuando haya q manejar input en el cliente, se le pasa el input desde le manejador de input al manejador de conexion. o se manda a cliente para q procese primerop si es necesario y d3sp el manejador de conexion
}

void Cliente::enviarMensajeAlChat() {
	//char* datosAEnviar = String(Comando::LOG +  cin >> mensaje).c_str();
	//int tamanio = String(Comando::LOG + cin >> mensaje).size();
	////this->conexionDelCliente->pasarDatosAEnviar(datosAEnviar, tamanio);
}

void Cliente::enviarMensajePrivado() {

}

void Cliente::procesarDatosRecibidos() {
	while (clienteActivo) {
		char* datosRecibidos = this->conexionDelCliente->getDatosRecibidos();


		//se copia primer byte
		//switch( primer byte)  hacer y hacer 
	}
}

