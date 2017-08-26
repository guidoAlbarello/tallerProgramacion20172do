#include "ServidorDeCorreo.h"

ServidorDeCorreo::ServidorDeCorreo() {
	Servidor();
}

void ServidorDeCorreo::correrCicloPrincipal() {
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
			mostrarUsuarioConectados();
			break;
		case '4':
			servidorActivo = false;
			break;
		default:
			break;
		}
	}
}

void ServidorDeCorreo::cerrarTodasLasConexiones() {
}

void ServidorDeCorreo::cambiarNivelLogeo() {
}

void ServidorDeCorreo::mostrarUsuarioConectados() {
}

void ServidorDeCorreo::mostrarMenu() {
	std::cout << "Menu" << std::endl;
	std::cout << "1.Close-server" << std::endl;
	std::cout << "2.Change-log-level" << std::endl;
	std::cout << "3.Show-connected-users" << std::endl;
	std::cout << "4.Exit" << std::endl;
}


