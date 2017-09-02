#include "ClienteDeChat.h"

ClienteDeChat* ClienteDeChat::instance = 0;

ClienteDeChat::ClienteDeChat() {
	Cliente();
}

ClienteDeChat * ClienteDeChat::getInstance() {
	if (!instance) {
		instance = new ClienteDeChat();
	}

	return instance;
}

void ClienteDeChat::iniciarCliente() {
	/*ParserXml xmlParser;
	clientConfig = xmlParser.readClientConfigFile(DEFAULT_USER_CONFIG_FILE);*/
	this->leerClientConfig();

	correrCicloPrincipal();
}

void ClienteDeChat::correrCicloPrincipal() {
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

void ClienteDeChat::mostrarMenu() {
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

void ClienteDeChat::conectarseAlServidor() {
	this->conexionDelCliente->iniciarConexion(clientConfig->getIP(), clientConfig->getPuerto().c_str());
	this->t_procesarDatosRecibidos = std::thread(&ClienteDeChat::procesarDatosRecibidos, this);
}

void ClienteDeChat::desconectarseDelServidor() {
	this->conexionDelCliente->cerrarConexion();
}

void ClienteDeChat::hacerTestDeEstres() {

}

void ClienteDeChat::revisarBuzon() {
	//this->conexionDelCliente->ejecutarComando(Comando::RETRIEVE_MESSAGES) 
}

void ClienteDeChat::logearseAlServidor() {
	//this->conexionDelCliente->ejecutarComando(Comando::LOG, user, pass)
	//char* datosAEnviar = String(Comando::LOG + cliente->usuario + cliente->pass).c_str();
	//int tamanio = String(Comando::LOG + cliente->usuario + cliente->pass).size();
	////this->conexionDelCliente->pasarDatosAEnviar(datosAEnviar, tamanio);

	//mutex en el buffer de manejar conexion .- cuando haya q manejar input en el cliente, se le pasa el input desde le manejador de input al manejador de conexion. o se manda a cliente para q procese primerop si es necesario y d3sp el manejador de conexion
}

void ClienteDeChat::enviarMensajeAlChat() {
	//char* datosAEnviar = String(Comando::LOG +  cin >> mensaje).c_str();
	//int tamanio = String(Comando::LOG + cin >> mensaje).size();
	////this->conexionDelCliente->pasarDatosAEnviar(datosAEnviar, tamanio);
}

void ClienteDeChat::enviarMensajePrivado() {

}

void ClienteDeChat::procesarDatosRecibidos() {
	while (clienteActivo) {
		char* datosRecibidos = this->conexionDelCliente->getDatosRecibidos();

		//se copia primer byte
		//switch( primer byte)  hacer y hacer 
	}
}

