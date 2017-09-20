#include "Cliente.h"
#include "../lib/rapidxml-1.13/rapidxml.hpp"
#include "../lib/rapidxml-1.13/rapidxml_print.hpp"
#include <fstream>
#include <chrono>

Cliente* Cliente::instance = 0;

Cliente * Cliente::getInstance() {
	if (!instance) {
		instance = new Cliente();
	}

	return instance;
}

Cliente::Cliente() {
	this->conexionDelCliente = new ManejadorDeConexionCliente();
	this->clienteActivo = true;
	this->estaLogueado = false;
	this->conexionViva = false;
	this->configuracion = new ClientConfig();
	this->buzonDeMensajesGlobales = new BuzonDeMensajes(); //LIBERAR LA MEMORIA  DEL BUZON
	this->enviandoMensaje = false;

	//Seteando el nombre del log
	Logger::getInstance()->setLogFileName(CLIENT_LOG_FILENAME_FORMAT);
}

Cliente::~Cliente() {
	delete this->conexionDelCliente;
}

void Cliente::cerrarCliente() {
	this->clienteActivo = false;


	if (this->conexionDelCliente->getConexionActiva()) {
		this->desconectarseDelServidor();
	}
}

void Cliente::leerClientConfig() {
	this->configuracion->leerConfiguracion();
}


void Cliente::iniciarCliente() {
	Logger::getInstance()->log(Debug, "Iniciando cliente...");
	this->leerClientConfig();
	this->correrCicloPrincipal();
}



void Cliente::correrCicloPrincipal() {
	std::string input;
	while (clienteActivo) {
		mostrarMenuPrincipal();
		std::getline(std::cin, input);
		if (input.length() != 1) {
			std::cout << "Debe ingresar una de las opciones" << std::endl;
		} else {
			char opcionElegida = input[0];
			Logger::getInstance()->log(Actividad, "Se ingresa la opcion " + input + " en el menu del cliente");
			switch (opcionElegida) {
			case '1':
				conectarseAlServidor();
				break;
			case '2':
				desconectarseDelServidor();
				break;
			case '3':
				cerrarCliente();
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
			case '9':
				this->conexionDelCliente->realizarPeticionUsuarios();
				break;
				/*case '0':
					cout << "IP " << this->configuracion->getIP() << " PUERTO: " << this->configuracion->getPuerto() << " PATH: " << this->configuracion->getPath() << endl;
					break;*/
			default:
				break;
			}
		}

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
	std::cout << "9.Usuarios Logueados" << std::endl;
}

void Cliente::conectarseAlServidor() {
	if (!this->conexionViva) {
		Logger::getInstance()->log(Debug, "Conectando al servidor...");
		std::cout << "Conectando al servidor..." << std::endl;
		this->conexionDelCliente->iniciarConexion(configuracion->getIP(), configuracion->getPuerto());
		this->conexionViva = true;
		this->t_procesarPing = std::thread(&Cliente::enviarPingAServidor, this);
		this->t_procesarDatosRecibidos = std::thread(&Cliente::procesarDatosRecibidos, this);
	}
	//}
	//else {
	//	Logger::getInstance()->log(Debug, "Conectando al servidor...");
	//	std::cout << "Ya se encuentra conectado al servidor" << std::endl;
	//}
}

void Cliente::desconectarseDelServidor() {
	Logger::getInstance()->log(Debug, "Desconectando del servidor...");
	std::cout << "Desconectando del servidor..." << std::endl;

	this->estaLogueado = false;
	this->conexionViva = false;
	try {
		if (t_procesarDatosRecibidos.joinable()) {
			t_procesarDatosRecibidos.join();
		}

		if (this->t_procesarPing.joinable()) {
			t_procesarPing.join();
		}
	} catch (exception e) {

	}

	if (this->conexionDelCliente != NULL) {
		this->conexionDelCliente->cerrarConexion();
	}
}

void Cliente::hacerTestDeEstres() {
	std::string stressFileName = this->configuracion->getPath();
	if (!existeArchivo(stressFileName)) {
		std::cout << "No existe el archivo de Test de Stress definido en la configuracion de usuario" << endl;
		Logger::getInstance()->log(Error, "No se pudo encontrar el archivo " + this->configuracion->getPath() + " para comenzar el test de stress");
		return;
	}
	std::cout << "Ingrese la cantidad de milisegundos para la prueba" << endl;
	std::string s_stressTimeMillis;
	int TEST_MAX_DURATION = 99999999;
	int stressTimeMillis = 0;
	while (!(cin >> stressTimeMillis)) {
		cin.clear();
		cin.ignore(TEST_MAX_DURATION, '\n');
		cout << "Ingrese solo numeros. Intente nuevamente: ";
	}
	std::cout << "Ingreso: " << stressTimeMillis << "ms" << endl;

	Logger::getInstance()->log(Actividad, "Ejecutando test de estres durante los proximos " + to_string(stressTimeMillis) + "ms");

	leerTestXML(stressFileName, stressTimeMillis);
}

void Cliente::leerTestXML(std::string stressFileName, int stressTimeMillis) {
	//Inicio el cronometro
	auto tComienzo = std::chrono::high_resolution_clock::now();
	std::chrono::milliseconds tConsumidoMs(0);

	Logger::getInstance()->log(Actividad, "[Cliente.cpp] Leyendo el archivo de test " + this->configuracion->getPath());

	try {
		rapidxml::xml_document<> documento;
		ifstream archivo(stressFileName);
		vector<char> buffer((istreambuf_iterator<char>(archivo)), istreambuf_iterator<char>());
		buffer.push_back('\0');
		documento.parse<0>(&buffer[0]); // <0> == sin flags de parseo

		rapidxml::xml_node<>* nodoStressTest = documento.first_node("StressTest");
		if (nodoStressTest != NULL) {
			for (rapidxml::xml_node<>* unNodoMensaje = nodoStressTest->first_node("mensaje"); unNodoMensaje && (tConsumidoMs.count() <= stressTimeMillis); unNodoMensaje = unNodoMensaje->next_sibling("mensaje")) {
				//Tiempo consumido -> el actual - el de comienzo
				tConsumidoMs = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - tComienzo);
				std::string nuevoMensaje = unNodoMensaje->value();
				this->conexionDelCliente->enviarMensajeGlobal(nuevoMensaje);
			}
			Logger::getInstance()->log(Actividad, "[Cliente.cpp] Finalizo el test de stress");
		}
		else {
			Logger::getInstance()->log(LogMode::Error, "[Cliente.cpp] No se encontro el nodo 'StressTest' en el archivo de test XML. No se puede realizar el test de estres.");
		}
	} catch (std::exception& e) {
		Logger::getInstance()->log(LogMode::Error, "[Cliente.cpp] Hubo un error al parsear el archivo de configuracion del servidor.");
		cout << e.what();
	}
}

bool Cliente::existeArchivo(const std::string& nombreDeArchivo) {
	std::ifstream archivo(nombreDeArchivo.c_str());
	return (bool)archivo;
}

void Cliente::revisarBuzon() {
	mostrarMenuBuzon();
	Logger::getInstance()->log(Debug, "Mensajes recibidos: ");
	std::cout << "Mensajes recibidos: " << std::endl;

	this->conexionDelCliente->devolverMensajesPrivados();
}

void Cliente::logearseAlServidor() {
	//if (!this->conexionDelCliente->getConexionActiva()) {
	//	Logger::getInstance()->log(Debug, "Se intento loguear al servidor un cliente sin conectarse previamente");
	//	cout << "Debe conectarse al servidor antes de loguearse" << std::endl;
	//}
	//else {
	while (!this->estaLogueado) {
		std::string user;
		std::string pass;
		mostrarMenuLogin();
		std::cout << "Ingrese su nombre de usuario" << std::endl;
		std::getline(std::cin, user);
		std::cout << "Ingrese su contrasenia" << std::endl;
		std::getline(std::cin, pass);

		Logger::getInstance()->log(Debug, "Logueando al servidor con credenciales: ");
		Logger::getInstance()->log(Debug, "Usuario: " + user);
		Logger::getInstance()->log(Debug, "Contrasenia: " + pass);

		estaLogueado = this->conexionDelCliente->login(user, pass);
		//mutex en el buffer de manejar conexion .- cuando haya q manejar input en el cliente, se le pasa el input desde le manejador de input al manejador de conexion. o se manda a cliente para q procese primerop si es necesario y d3sp el manejador de conexion
	}
	//}
	correrCicloPrincipal();
}

void Cliente::enviarMensajeAlChat() {
	this->enviandoMensaje = true;
	mostrarMenuMensajeChat();
	std::cout << "Ingrese el mensaje a enviar al chat" << std::endl;
	std::string mensaje;
	std::getline(std::cin, mensaje);

	if (this->conexionDelCliente->enviarMensajeGlobal(mensaje)) {
		Logger::getInstance()->log(Debug, "Se envio al chat el mensaje: " + mensaje);
	} else {
		Logger::getInstance()->log(Debug, "No se pudo enviar al chat el mensaje: " + mensaje);
	}
	this->enviandoMensaje = false;
}

void Cliente::enviarMensajePrivado() {
	this->enviandoMensaje = true;
	mostrarMenuMensajePrivado();
	std::cout << "Ingrese el destinatario" << std::endl;
	std::string destinatario;
	std::getline(std::cin, destinatario);
	std::cout << "Ingrese el mensaje a enviar" << std::endl;
	std::string mensaje;
	std::getline(std::cin, mensaje);

	if (this->conexionDelCliente->enviarMensajePrivado(destinatario, mensaje)) {
		std::string mensajeLogueado = "Se envio el mensaje privado: " + mensaje;
		mensajeLogueado.append(" con destinatario: " + destinatario);
		Logger::getInstance()->log(Debug, mensajeLogueado);
	} else {
		std::string mensajeLogueado = "No se pudo enviar el mensaje privado: " + mensaje;
		mensajeLogueado.append(" con destinatario: " + destinatario);
		Logger::getInstance()->log(Debug, mensajeLogueado);
	}
	this->enviandoMensaje = false;
}


void Cliente::mostrarMensajesPrivados(MensajeDeRed * unMensajeDeRed) {
	if (unMensajeDeRed->getParametro(0).compare("RESULTADO_RETRIEVE_MESSAGES_OK") == 0) {
		for (int i = 1; i < unMensajeDeRed->getCantidadDeParametros(); i++) {
			string unEmisor = unMensajeDeRed->getParametro(i);
			string unMensaje = unMensajeDeRed->getParametro(++i);

			std::cout << "[" + unEmisor + "]: " + unMensaje << endl;
		}
	} else {

	}
}

void Cliente::mostrarUsuariosConectados(MensajeDeRed * unMensajeDeRed) {
	std::cout << "|----------------------------|" << std::endl;
	std::cout << "|     Usuarios Logueados     |" << std::endl;
	std::cout << "|----------------------------|" << std::endl;
	int i;
	for (i = 0; i < unMensajeDeRed->getCantidadDeParametros(); i++) {
		std::cout << unMensajeDeRed->getParametro(i) << std::endl;
	}
}

void Cliente::procesarMensajesGlobales(MensajeDeRed * unMensajeDeRed) {
	if (unMensajeDeRed->getParametro(0).compare("RECIEVE_GLOBAL_MESSAGES_OK") == 0) {
		Logger::getInstance()->log(Debug, "se procesa un recieve_global_messages_ok");
		for (int i = 1; i < unMensajeDeRed->getCantidadDeParametros(); i++) {
			string unEmisor = unMensajeDeRed->getParametro(i);
			string unMensaje = unMensajeDeRed->getParametro(++i);
			buzonDeMensajesGlobales->recibirMensaje("/global", unEmisor, unMensaje);
		}
	} else {

	}
}

void Cliente::procesarMensajesPrivados(MensajeDeRed * unMensajeDeRed) {
	if (unMensajeDeRed->getParametro(0).compare("RECIEVE_PRIVATE_MESSAGES_OK") == 0) {
		Logger::getInstance()->log(Debug, "se procesa un recieve_private_messages_ok");
		for (int i = 1; i < unMensajeDeRed->getCantidadDeParametros(); i++) {
			string unEmisor = unMensajeDeRed->getParametro(i);
			string unMensaje = unMensajeDeRed->getParametro(++i);
			buzonDeMensajesGlobales->recibirMensaje("/private", unEmisor, unMensaje);
		}
	} else {

	}
}


/* Procesa comandos recibidos desde el servidor */
void Cliente::procesarDatosRecibidos() {
	auto timeOut = std::chrono::high_resolution_clock::now();

	while (this->conexionViva) {
		char* datosRecibidos = this->conexionDelCliente->getMensaje();
		if (datosRecibidos != NULL) {
			timeOut = std::chrono::high_resolution_clock::now();
			std::string datosRecibidosString(datosRecibidos);
			MensajeDeRed* mensajeDeRed = new MensajeDeRed(datosRecibidosString, Constantes::CLIENTE); //LIBERAR este mensaje de red, no se borra nunca!!!!!!!!!

			switch (mensajeDeRed->getComandoCliente()) {
			case ComandoCliente::RESULTADO_LOGIN:
				procesarResultadoLogin(mensajeDeRed);
				break;
			case ComandoCliente::RESULTADO_SEND_MESSAGE:
				procesarResultadoSendMessage(mensajeDeRed);
				break;
			case ComandoCliente::RECIEVE_GLOBAL_MESSAGES:
				procesarMensajesGlobales(mensajeDeRed);
				break;
			case ComandoCliente::RESULTADO_RETRIEVE_MESSAGES:
				mostrarMensajesPrivados(mensajeDeRed);
				break;
			case ComandoCliente::RECIEVE_PRIVATE_MESSAGES:
				procesarMensajesPrivados(mensajeDeRed);
				break;
			case ComandoCliente::LOG:
				Logger::getInstance()->log(Debug, "Recibio un LOG");
				Logger::getInstance()->log(Debug, datosRecibidosString);
				break;
			case ComandoCliente::PRINT:
				Logger::getInstance()->log(Debug, "Recibio un PRINT");
				std::cout << datosRecibidosString << endl;
				break;
			case ComandoCliente::VACIO:
				Logger::getInstance()->log(Debug, "Recibio un VACIO");
				break;
			case ComandoCliente::RESULTADO_PING:
				Logger::getInstance()->log(Debug, "Recibio un RESULTADO_PING");
				break;
			case ComandoCliente::RESULTADO_USUARIOS:
				Logger::getInstance()->log(Debug, "se recibio una lista de usuarios");
				mostrarUsuariosConectados(mensajeDeRed);
				break;
			default:
				Logger::getInstance()->log(Debug, datosRecibidos);
			}
			if (datosRecibidos != NULL)
				free(datosRecibidos);
			if (mensajeDeRed != NULL)
				delete mensajeDeRed;
		}
		if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - timeOut).count() > (Constantes::PING_DELAY + 1000 * 3)) {
			this->desconectarseDelServidor();
			Logger::getInstance()->log(Debug, "Se desconecto un cliente del servidor por falta de respuesta al ping");
			std::cout << "Se ha desconectado del servidor" << std::endl;
		}
		mostrarMensajesGlobales();
	}
}

void Cliente::mostrarMensajesGlobales() {
	int i;
	for (i = 0; i < this->buzonDeMensajesGlobales->getTamanio() && !enviandoMensaje; i++) {
		Mensaje* unMensaje = this->buzonDeMensajesGlobales->verMensaje(i);

		std::cout << unMensaje->getDestinatario() + " [" + unMensaje->getEmisor() + "]: " + unMensaje->getMensaje() << endl;

	}

	if (i > 0)
		this->buzonDeMensajesGlobales->eliminarMensajes(i); //esto de mostrar se peue hacer en otro thread si tira problemas e performance
}

void Cliente::procesarResultadoSendMessage(MensajeDeRed* mensajeDeRed) {
	if (mensajeDeRed->getParametro(0) == "SEND_MESSAGE_OK") {
		// Mensaje enviado satisfactoriamente
		cout << mensajeDeRed->getParametro(1) << endl;
	} else if (mensajeDeRed->getParametro(0) == "SEND_MESSAGE_NOK") {
		// Fallo el envio del mensaje
		std::cout << mensajeDeRed->getParametro(1) << endl;
	}
}

void Cliente::procesarResultadoLogin(MensajeDeRed* mensajeDeRed) {
	if (mensajeDeRed->getParametro(0) == "LOGIN_OK") {
		this->estaLogueado = true;
		cout << mensajeDeRed->getParametro(1) << endl;
	} else if (mensajeDeRed->getParametro(0) == "LOGIN_NOK") {
		this->estaLogueado = false;
		cout << mensajeDeRed->getParametro(1) << endl;
	}
}

void Cliente::enviarPingAServidor() {
	while (this->conexionViva) {
		Logger::getInstance()->log(Debug, "Cliente enviando PING al servidor");
		this->conexionDelCliente->enviarSolicitudPing();
		std::this_thread::sleep_for(std::chrono::milliseconds(Constantes::PING_DELAY));
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

