#include "Cliente.h"
#include <iostream>
#include <limits>
#include "Logger.h"

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
	this->configuracion = new ClientConfig();
}

Cliente::~Cliente() {
	delete this->conexionDelCliente;
}

void Cliente::cerrarCliente() {
	this->conexionDelCliente->cerrarConexion();
	this->clienteActivo = false;
}

void Cliente::leerClientConfig() {
	this->configuracion->leerConfiguracion();
}


void Cliente::iniciarCliente() {
	this->leerClientConfig();

	this->correrCicloPrincipal();
}

void Cliente::correrCicloPrincipal() {
	std::string input;
	while (clienteActivo) {
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
					conectarseAlServidor();
					break;
				case '2':
					desconectarseDelServidor();
					break;
				case '3':
					clienteActivo = false;
					desconectarseDelServidor();
					break;
				case '4':
					logearseAlServidor();
					break;
				case '5':
					hacerTestDeEstres();
					break;
				case '6':
					revisarBuzon();
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
		correrCicloPrincipal();
	}
}

void Cliente::mostrarMenuPrincipal() {
	cout << "|----------------------------|" << std::endl;
	cout << "|        Menu cliente        |" << std::endl;
	cout << "|----------------------------|" << std::endl;
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
	std::cout << "Conectando al servidor..." << std::endl;
	this->conexionDelCliente->iniciarConexion(configuracion->getIP(), configuracion->getPuerto());
	this->t_procesarDatosRecibidos = std::thread(&Cliente::procesarDatosRecibidos, this);
}

void Cliente::desconectarseDelServidor() {
	std::cout << "Desconectando del servidor..." << std::endl;
	this->conexionDelCliente->cerrarConexion();
}

void Cliente::hacerTestDeEstres() {
	std::cout << "Ingrese la cantidad de milisegundos para la prueba" << std::endl;
	std::string s_stressTimeMillis;
	int TEST_MAX_DURATION = 99999999;
	int stressTimeMillis = 0;
	while (!(cin >> stressTimeMillis)) {
		cin.clear();
		cin.ignore(TEST_MAX_DURATION, '\n');
		cout << "Ingrese solo numeros.  Intente nuevamente: ";
	}
	cout << "Ingreso: " << stressTimeMillis << "ms" << endl;

	// TODO: implementar el test de estres
}

void Cliente::revisarBuzon() {
	mostrarMenuBuzon();
	std::cout << "Mensajes recibidos: " << std::endl;

	// TODO: implementar llamado al server que traiga los msjs privados
	//this->conexionDelCliente->ejecutarComando(Comando::RETRIEVE_MESSAGES) 

}

void Cliente::logearseAlServidor() {
	bool loginOk = false;
	while (!loginOk) {
		std::string user;
		std::string pass;
		mostrarMenuLogin();
		cout << "Ingrese su nombre de usuario" << std::endl;
		std::getline(std::cin, user);
		cout << "Ingrese su contrasenia" << std::endl;
		std::getline(std::cin, pass);

		//this->conexionDelCliente->ejecutarComando(Comando::LOG, user, pass)
		this->conexionDelCliente->login(user, pass);
		//char* datosAEnviar = String(Comando::LOG + cliente->usuario + cliente->pass).c_str();
		//int tamanio = String(Comando::LOG + cliente->usuario + cliente->pass).size();
		////this->conexionDelCliente->pasarDatosAEnviar(datosAEnviar, tamanio);

		cout << "El login fue satisfactorio" << std::endl;
		loginOk = true;

		// cout << "Los datos ingresados son incorrectos" << std::endl;
		// break;

		//mutex en el buffer de manejar conexion .- cuando haya q manejar input en el cliente, se le pasa el input desde le manejador de input al manejador de conexion. o se manda a cliente para q procese primerop si es necesario y d3sp el manejador de conexion
	}
	correrCicloPrincipal();
}

void Cliente::enviarMensajeAlChat() {
	mostrarMenuMensajeChat();
	std::cout << "Ingrese el mensaje a enviar al chat" << std::endl;
	std::string mensaje;
	std::getline(std::cin, mensaje);

	//char* datosAEnviar = String(Comando::LOG +  cin >> mensaje).c_str();
	//int tamanio = String(Comando::LOG + cin >> mensaje).size();
	////this->conexionDelCliente->pasarDatosAEnviar(datosAEnviar, tamanio);
}

void Cliente::enviarMensajePrivado() {
	mostrarMenuMensajePrivado();
	std::cout << "Ingrese el destinatario" << std::endl;
	std::string destinatario;
	std::getline(std::cin, destinatario);
	std::cout << "Ingrese el mensaje a enviar" << std::endl;
	std::string mensaje;
	std::getline(std::cin, mensaje);
}

void Cliente::procesarDatosRecibidos() {
	while (clienteActivo) {
		char* datosRecibidos = this->conexionDelCliente->getDatosRecibidos();
		if (datosRecibidos != NULL) {
			Logger::getInstance()->log(Debug, datosRecibidos);
		}

		//se copia primer byte
		//switch( primer byte)  hacer y hacer 
	}
}

void Cliente::mostrarMenuLogin() {
	cout << "|----------------------------|" << std::endl;
	cout << "|            Login           |" << std::endl;
	cout << "|----------------------------|" << std::endl;
}

void Cliente::mostrarMenuBuzon() {
	cout << "|----------------------------|" << std::endl;
	cout << "|            Buzon           |" << std::endl;
	cout << "|----------------------------|" << std::endl;
}

void Cliente::mostrarMenuMensajeChat() {
	cout << "|----------------------------|" << std::endl;
	cout << "|        Mensaje Chat        |" << std::endl;
	cout << "|----------------------------|" << std::endl;
}

void Cliente::mostrarMenuMensajePrivado() {
	cout << "|----------------------------|" << std::endl;
	cout << "|       Mensaje Privado      |" << std::endl;
	cout << "|----------------------------|" << std::endl;
}

