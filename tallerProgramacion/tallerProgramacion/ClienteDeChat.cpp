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
	ParserXml xmlParser;
	clientConfig = xmlParser.readClientConfigFile(DEFAULT_USER_CONFIG_FILE);

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
	this->conexionDelCliente->iniciarConexionCliente(clientConfig->getIP(), std::to_string(clientConfig->getPuerto()));
}

void ClienteDeChat::desconectarseDelServidor() {
	this->conexionDelCliente->cerrarConexion();
}

void ClienteDeChat::hacerTestDeEstres() {

}

void ClienteDeChat::revisarBuzon() {

}

void ClienteDeChat::logearseAlServidor() {
}

void ClienteDeChat::enviarMensajeAlChat() {
}

void ClienteDeChat::enviarMensajePrivado() {
}
