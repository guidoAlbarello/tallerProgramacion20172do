#include "Cliente.h"
#include "../lib/rapidxml-1.13/rapidxml.hpp"
#include "../lib/rapidxml-1.13/rapidxml_print.hpp"
#include <fstream>
#include <chrono>
#include "LoggerView.h"
#include "Juego.h"

Cliente* Cliente::instance = 0;

Cliente * Cliente::getInstance() {
	if (!instance) {
		instance = new Cliente();
	}

	return instance;
}

Cliente::Cliente() {
	this->clienteActivo = true;
	this->estaLogueado = false;
	this->conexionViva = false;
	this->configuracion = new ClientConfig();
	this->buzonDeMensajesGlobales = new BuzonDeMensajes(); //LIBERAR LA MEMORIA  DEL BUZON
	this->enviandoMensaje = false;
	this->maquinaDeEstados = new MaquinaEstados();
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
	this->correrCicloPrincipal();  //iniciarMaquinaEstados
}


void Cliente::correrCicloPrincipal() {
	std::string input;
	while (clienteActivo) {
		conectarseAlServidor();
		logearseAlServidor();

		if (this->estaLogueado) {
			std::cout << "Login exitoso" << std::endl;
		}
		else {
			std::cout << "Login fallido" << std::endl;
		}
		system("PAUSE");
		/*
		//mostrarMenuPrincipal();
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
			default:
				break;
			}
		}
		*/
	}
}

void Cliente::mostrarMenuPrincipal() {
	m_print.lock();
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
	m_print.unlock();
}

void Cliente::conectarseAlServidor() {
	if (!this->conexionViva) {
		Logger::getInstance()->log(Debug, "Conectando al servidor...");
		m_print.lock();
		std::cout << "Conectando al servidor..." << std::endl;
		m_print.unlock();
		this->conexionDelCliente = new ManejadorDeConexionCliente();
		if (t_procesarDatosRecibidos.joinable()) {
			t_procesarDatosRecibidos.join();
		}
		if (this->conexionDelCliente->iniciarConexion(configuracion->getIP(), configuracion->getPuerto())) {
			this->conexionViva = true;
			this->t_procesarPing = std::thread(&Cliente::enviarPingAServidor, this);
			this->t_procesarDatosRecibidos = std::thread(&Cliente::procesarDatosRecibidos, this);
			this->renderer = new Renderer();
			this->renderer->iniciarRenderer();
		}
		else {
			this->conexionViva = false;
			m_print.lock();
			std::cout << "No se pudo conectar al sevidor" << std::endl;
			m_print.unlock();
		}
	}
	else {
		m_print.lock();
		std::cout << "Ya se encuentra conectado al servidor" << std::endl;
		m_print.unlock();
	}
}

void Cliente::desconectarseDelServidor() {
	
	if (!this->conexionViva) {
		cout << "Usted no esta conectado con el servidor." << endl;
		return;
	}

	Logger::getInstance()->log(Debug, "Desconectando del servidor...");
	m_print.lock();
	std::cout << "Desconectando del servidor..." << std::endl;
	m_print.unlock();

	this->estaLogueado = false;
	this->conexionViva = false;
	try {
		if (this->t_procesarPing.joinable()) {
			t_procesarPing.join();
		}

		this->renderer->cerrarRenderer();
		delete this->renderer;
		if (this->conexionDelCliente != NULL) {
			this->conexionDelCliente->cerrarConexion();
			delete this->conexionDelCliente;
		}
	} catch (exception e) {
		Logger::getInstance()->log(Debug, "Ocurrio un error al desconectarse del servidor");
	}	
	
}

void Cliente::hacerTestDeEstres() {
	if (!this->estaLogueado) {
		m_print.lock();
		cout << "Necesita estar logueado para realizar esta accion." << endl;
		m_print.unlock();
		return;
	}

	std::string stressFileName = this->configuracion->getPath();
	if (!existeArchivo(stressFileName)) {
		m_print.lock();
		std::cout << "No existe el archivo de Test de Stress definido en la configuracion de usuario" << endl;
		m_print.unlock();
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
			if (tConsumidoMs.count() <= stressTimeMillis) {
				Logger::getInstance()->log(Actividad, "[Cliente.cpp] Finalizo el test de stress satisfactoriamente");
				m_print.lock();
				std::cout << "El test de stress finalizo satisfactoriamente" << std::endl;
				m_print.unlock();
			}
			else {
				Logger::getInstance()->log(Actividad, "[Cliente.cpp] Finalizo el test de stress sin enviar todos los mensajes");
				m_print.lock();
				std::cout << "El test de stress finalizo sin enviar todos los mensajes" << std::endl;
				m_print.unlock();
			}
		}
		else {
			Logger::getInstance()->log(LogMode::Error, "[Cliente.cpp] No se encontro el nodo 'StressTest' en el archivo de test XML. No se puede realizar el test de estres.");
		}
	} catch (std::exception& e) {
		Logger::getInstance()->log(LogMode::Error, "[Cliente.cpp] Hubo un error al parsear el archivo de configuracion del servidor.");
		m_print.lock();
		cout << e.what();
		m_print.unlock();
	}
}

bool Cliente::existeArchivo(const std::string& nombreDeArchivo) {
	std::ifstream archivo(nombreDeArchivo.c_str());
	return (bool)archivo;
}

void Cliente::revisarBuzon() {
	if (!this->estaLogueado) {
		cout << "Necesita estar logueado para realizar esta accion." << endl;
		return;
	}

	mostrarMenuBuzon();
	Logger::getInstance()->log(Debug, "Mensajes recibidos: ");
	m_print.lock();
	std::cout << "Mensajes recibidos: " << std::endl;
	m_print.unlock();

	this->conexionDelCliente->devolverMensajesPrivados();
}

void Cliente::logearseAlServidor() {
	if (!this->conexionViva) {
		cout << "Necesita estar conectado con el servidor para realizar esta accion." << endl;
		return;
	}

	if (this->estaLogueado) {
		cout << "Usted ya esta logueado." << endl;
		return;
	}
		

	LoggerView loggerView = LoggerView(this->renderer->getRenderer());


	Usuario* usuario = loggerView.showLogin();
	std::cout << "Nombre de usuario: " << usuario->getNombre() << std::endl;
	std::cout << "Pass: " << usuario->getPassword() << std::endl;

	estaLogueado = this->conexionDelCliente->login(usuario->getNombre(), usuario->getPassword());
	if (!estaLogueado) {
		std::cout << "Login fallido" << std::endl;
		return;
	}

	else {
		std::cout << "Login ok" << std::endl;
		Juego* juego = new Juego(this->renderer);
		juego->iniciarJuego();
	}
		/*
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

		if (estaLogueado) {
			renderer = new Renderer();
			renderer->iniciarRenderer();
		}
		//mutex en el buffer de manejar conexion .- cuando haya q manejar input en el cliente, se le pasa el input desde le manejador de input al manejador de conexion. o se manda a cliente para q procese primerop si es necesario y d3sp el manejador de conexion
		*/

	//}

	//correrCicloPrincipal();
}

void Cliente::enviarMensajeAlChat() {
	if (!this->estaLogueado) {
		cout << "Necesita estar logueado para realizar esta accion." << endl;
		return;
	}

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
	if (!this->estaLogueado) {
		cout << "Necesita estar logueado para realizar esta accion." << endl;
		return;
	}

	this->conexionDelCliente->realizarPeticionUsuarios();
	this->enviandoMensaje = true;
	mostrarMenuMensajePrivado();
	m_print.lock();
	std::cout << "Ingrese el destinatario" << std::endl;
	m_print.unlock();
	std::string destinatario;
	std::getline(std::cin, destinatario);
	m_print.lock();
	std::cout << "Ingrese el mensaje a enviar" << std::endl;
	m_print.unlock();
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
	m_print.lock();
	std::cout << "|----------------------------|" << std::endl;
	std::cout << "|     Usuarios Logueados     |" << std::endl;
	std::cout << "|----------------------------|" << std::endl;
	int i;
	for (i = 0; i < unMensajeDeRed->getCantidadDeParametros(); i++) {
		std::cout << unMensajeDeRed->getParametro(i) << std::endl;
	}
	m_print.unlock();
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
			Constantes::EstadoJuego estadoJuego;

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
			case ComandoCliente::UPDATE_MODEL:
				Logger::getInstance()->log(Debug, "Recibio un un UPDATE_MODEL");

				// TODO: este memcpy trae bien pero por alguna razon cuando sale del while explota, si se comenta se fixea
				memcpy(&estadoJuego, datosRecibidos, sizeof(Constantes::EstadoJuego));
				std::cout << "Estado juego:" << std::endl;
				std::cout << estadoJuego.estados[0].usuario << std::endl;

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
		/*
		if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - timeOut).count() > (Constantes::PING_DELAY + 1000 * 3)) {
			this->desconectarseDelServidor();
			Logger::getInstance()->log(Debug, "Se desconecto un cliente del servidor por falta de respuesta al ping");
			std::cout << "Se ha desconectado del servidor" << std::endl;
		}
		*/
		mostrarMensajesGlobales();
	}
}

void Cliente::mostrarMensajesGlobales() {
	int i;
	m_print.lock();
	for (i = 0; i < this->buzonDeMensajesGlobales->getTamanio() && !enviandoMensaje; i++) {
		Mensaje* unMensaje = this->buzonDeMensajesGlobales->verMensaje(i);

		std::cout << unMensaje->getDestinatario() + " [" + unMensaje->getEmisor() + "]: " + unMensaje->getMensaje() << endl;

	}
	m_print.unlock();

	if (i > 0)
		this->buzonDeMensajesGlobales->eliminarMensajes(i); //esto de mostrar se peue hacer en otro thread si tira problemas e performance
}

void Cliente::procesarResultadoSendMessage(MensajeDeRed* mensajeDeRed) {
	if (mensajeDeRed->getParametro(0) == "SEND_MESSAGE_OK") {
		// Mensaje enviado satisfactoriamente
		Logger::getInstance()->log(Debug, mensajeDeRed->getParametro(1));
	} else if (mensajeDeRed->getParametro(0) == "SEND_MESSAGE_NOK") {
		// Fallo el envio del mensaje
		Logger::getInstance()->log(Debug, mensajeDeRed->getParametro(1));
	}
}

void Cliente::procesarResultadoLogin(MensajeDeRed* mensajeDeRed) {
	if (mensajeDeRed->getParametro(0) == "LOGIN_OK") {
		this->estaLogueado = true;
		m_print.lock();
		cout << mensajeDeRed->getParametro(1) << endl;
		m_print.unlock();
	} else if (mensajeDeRed->getParametro(0) == "LOGIN_NOK") {
		this->estaLogueado = false;
		m_print.lock();
		cout << mensajeDeRed->getParametro(1) << endl;
		m_print.unlock();
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
	m_print.lock();
	cout << "|----------------------------|" << std::endl;
	cout << "|            Login           |" << std::endl;
	cout << "|----------------------------|" << std::endl;
	m_print.unlock();
}

void Cliente::mostrarMenuBuzon() {
	m_print.lock();
	cout << "|----------------------------|" << std::endl;
	cout << "|            Buzon           |" << std::endl;
	cout << "|----------------------------|" << std::endl;
	m_print.unlock();
}

void Cliente::mostrarMenuMensajeChat() {
	m_print.lock();
	cout << "|----------------------------|" << std::endl;
	cout << "|        Mensaje Chat        |" << std::endl;
	cout << "|----------------------------|" << std::endl;
	m_print.unlock();
}

void Cliente::mostrarMenuMensajePrivado() {
	m_print.lock();
	cout << "|----------------------------|" << std::endl;
	cout << "|       Mensaje Privado      |" << std::endl;
	cout << "|----------------------------|" << std::endl;
	m_print.unlock();
}

