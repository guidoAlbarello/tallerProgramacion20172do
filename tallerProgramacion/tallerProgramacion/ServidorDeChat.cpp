#include "ServidorDeChat.h"

ServidorDeChat* ServidorDeChat::instance = 0;

ServidorDeChat * ServidorDeChat::getInstance() {
	if (!instance) {
		instance = new ServidorDeChat();
	}

	return instance;
}

Usuario * ServidorDeChat::buscarUsuario(std::string unUsuario) {
	Usuario* usuarioDestinatario = NULL;
	
	for (int i = 0; i < listaDeUsuarios.size() && usuarioDestinatario == NULL; i++) {
		if (unUsuario.compare(listaDeUsuarios[i]->getNombre()) == 0)
			usuarioDestinatario = listaDeUsuarios[i];
	}

	return usuarioDestinatario;
}

ServidorDeChat::ServidorDeChat() {
	Servidor();
}

void ServidorDeChat::correrCicloPrincipal() {
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
		default:
			break;
		}
	}
}

void ServidorDeChat::cerrarTodasLasConexiones() {
	for (int i = 0; i < conexionesActivas.size(); i++) {
		conexionesActivas[i]->cerrarConexion();
	}
}

void ServidorDeChat::cambiarNivelLogeo() {
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

void ServidorDeChat::mostrarUsuariosConectados() {
	std::cout << "-------------------------------" << std::endl;
	std::cout << "Usuarios conectados" << std::endl;
	std::cout << "-------------------------------" << std::endl;
	
	for (int i = 0; i < conexionesActivas.size(); i++) {
		std::cout << "Usuario: " << conexionesActivas[i]->getUsuario()->getNombre() << std::endl;
	}
}

void ServidorDeChat::mostrarMenu() {
	std::cout << "-------------------------------" << std::endl;
	std::cout << "Menu" << std::endl;
	std::cout << "-------------------------------" << std::endl;
	std::cout << "1.Close-server" << std::endl;
	std::cout << "2.Change-log-level" << std::endl;
	std::cout << "3.Show-connected-users" << std::endl;
	std::cout << "4.Exit" << std::endl;
}


