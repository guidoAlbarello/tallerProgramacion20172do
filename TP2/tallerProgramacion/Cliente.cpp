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

void Cliente::render() {
	Uint32 inicio = 0;
	Uint32 fin = 0;
	Uint32 intervalo = 0;

	while (clienteActivo) {
		if (!dibujarGameOver) {
			inicio = SDL_GetTicks();
			if (this->renderer != NULL) {
				this->maquinaDeEstados->render();
			}
			fin = SDL_GetTicks();
			intervalo = fin - inicio;
			if (intervalo > 1000 / Constantes::FPS) {
				intervalo = (1000 / Constantes::FPS) - 1;
			}
		} else {
			SDL_SetRenderDrawColor(this->renderer->getRendererJuego(), 242, 242, 242, 255);
			SDL_RenderClear(this->renderer->getRendererJuego());
			Ltexture* gBackgroundImage = new Ltexture(this->renderer->getRendererJuego());
			gBackgroundImage->loadFromFile("imagenes/game_over.bmp");
			SDL_Rect* rectanguloFullscreen = new SDL_Rect();
			rectanguloFullscreen->w = 800;
			rectanguloFullscreen->h = 600;
			gBackgroundImage->render(0, 0, rectanguloFullscreen);
			SDL_RenderPresent(this->renderer->getRendererJuego());

			std::this_thread::sleep_for(std::chrono::milliseconds(1000 * 5)); 
			clienteActivo = false;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds((1000/Constantes::FPS) - intervalo)); // por ahora se comenta, esto mejora mucho la fluidez
	}
}

Cliente::Cliente() {
	/*int a = 1414877262;
	char b[5]; 
	memcpy(b, &a, sizeof(int));
	b[4] = '\0';
	cout << b << endl;
	*/
	this->clienteActivo = true;
	this->estaLogueado = false;
	this->conexionViva = false;
	this->configuracion = new ClientConfig();
	this->buzonDeMensajesGlobales = new BuzonDeMensajes(); //LIBERAR LA MEMORIA  DEL BUZON
	this->enviandoMensaje = false;
	this->maquinaDeEstados = new MaquinaEstados();
	this->juegoIniciado = false;
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
	conectarseAlServidor();
	if (!this->conexionViva) {
		cout << "No se pudo conectar al servidor. Maxima cantidad de clientes conectados." << endl;
		this->cerrarCliente();
		return;
	}
	cout << "Se ha conectado satisfactoriamente al servidor" << endl;
	if (this->estaLogueado) {
		cout << "Usted ya esta logueado." << endl;
		return;
	}

	this->maquinaDeEstados->changeState(new EstadoLogeo());
	this->t_render = std::thread(&Cliente::render, this);
	this->correrCicloPrincipal();
}

void Cliente::correrCicloPrincipal() {
	Uint32 inicio = 0;
	Uint32 fin = 0;
	Uint32 intervalo = 0;

	while (clienteActivo) {
		this->maquinaDeEstados->update(this->conexionDelCliente);
		if (ManejadorInput::getInstance()->cerrarCliente()) {
			this->clienteDecideCerrar = true;
			clienteActivo = false;
		}
		if (!juegoIniciado || enPantallaDeTransicion) {
			enviandoPing = true;
			fin = SDL_GetTicks();
			intervalo = fin - inicio;
			if (intervalo > Constantes::PING_DELAY) {
				this->conexionDelCliente->enviarSolicitudPing();
				intervalo = 0;
			}
		}
		
		//std::this_thread::sleep_for(std::chrono::milliseconds(1000 / (Constantes::FPS)));
		std::this_thread::sleep_for(std::chrono::milliseconds(25));
	}
	this->cerrarCliente();
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
			this->renderer->iniciarRendererJuego();
			renderer->iniciarRendererMapa(); // Se inicia la ventana del minimapa
			this->maquinaDeEstados->setRenderer(this->renderer);
		} else {
			this->conexionViva = false;
			if (t_render.joinable()) {
				t_render.join();
			}
			m_print.lock();
			std::cout << "No se pudo conectar al sevidor" << std::endl;
			m_print.unlock();
			this->cerrarCliente();
		}
	} else {
		m_print.lock();
		std::cout << "Ya se encuentra conectado al servidor" << std::endl;
		m_print.unlock();
	}
}

void Cliente::desconectarseDelServidor() {
	/*if (!this->conexionViva) {
		cout << "Usted no esta conectado con el servidor." << endl;
		return;
	}
	*/
	Logger::getInstance()->log(Debug, "Desconectando del servidor...");
	m_print.lock();
	std::cout << "Desconectando del servidor..." << std::endl;
	m_print.unlock();

	this->estaLogueado = false;
	this->conexionViva = false;
	this->clienteActivo = false;
	if(clienteDecideCerrar)
		this->conexionDelCliente->desconectarServidor();
	try {
		if (this->t_render.joinable()) {
			t_render.join();
		}
		this->maquinaDeEstados->popState();
		this->renderer->cerrarRenderer();
		delete this->maquinaDeEstados;
		delete this->renderer;
		if (this->t_procesarPing.joinable()) {
			t_procesarPing.join();
		}		
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
			} else {
				Logger::getInstance()->log(Actividad, "[Cliente.cpp] Finalizo el test de stress sin enviar todos los mensajes");
				m_print.lock();
				std::cout << "El test de stress finalizo sin enviar todos los mensajes" << std::endl;
				m_print.unlock();
			}
		} else {
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
	/*

	LoggerView loggerView = LoggerView(this->renderer->getRendererJuego());


	Usuario* usuario = loggerView.showLogin();
	std::cout << "Nombre de usuario: " << usuario->getNombre() << std::endl;
	std::cout << "Pass: " << usuario->getPassword() << std::endl;
	//esto tiene q ir a la parte del update del estado, , donde eliminar el usuario ?= 
	estaLogueado = this->conexionDelCliente->login(usuario->getNombre(), usuario->getPassword());
	if (!estaLogueado) {
		Logger::getInstance()->log(Debug, "El login fue incorrecto");
		return;
	}

	else {
		Logger::getInstance()->log(Debug, "El login fue satisfactorio");
		Logger::getInstance()->log(Debug, "Iniciando juego...");
		Juego* juego = new Juego();
		//juego->iniciarJuego();
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

	//correrCicloPrincipal();*/
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
			MensajeDeRed* mensajeDeRed = new MensajeDeRed(datosRecibidosString, Constantes::CLIENTE);
			EstadoModeloJuego* estadoModeloJuego = NULL;
			EstadoInicialJuego* estadoInicialJuego = NULL;
			bool resultado = false;
			switch (mensajeDeRed->getComandoCliente()) {
			case ComandoCliente::RESULTADO_LOGIN:
				procesarResultadoLogin(mensajeDeRed, datosRecibidos);
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
			case ComandoCliente::CLOSE:
				this->estaLogueado = false;
				this->conexionViva = false;
				this->clienteActivo = false;
				break;
			case ComandoCliente::PRINT:
				Logger::getInstance()->log(Debug, "Recibio un PRINT");
				std::cout << datosRecibidosString << endl;
				break;
			case ComandoCliente::INIT:
				Logger::getInstance()->log(Debug, "Recibio un INIT");
				enviandoPing = false;
				estadoInicialJuego = new EstadoInicialJuego();
				memcpy(estadoInicialJuego, datosRecibidos + 4 + 1, sizeof(EstadoInicialJuego));
				iniciarJuego(estadoInicialJuego);
				break;
			case ComandoCliente::UPDATE_MODEL:
				//Logger::getInstance()->log(Debug, "Recibio un UPDATE_MODEL");
				if (!enviandoPing) {
					enviandoPing = false;
					enPantallaDeTransicion = false;
					estadoModeloJuego = new EstadoModeloJuego();
					memcpy(estadoModeloJuego, datosRecibidos + 12 + 1, sizeof(EstadoModeloJuego));
					procesarEstadoModelo(estadoModeloJuego);
				}
				break;
			case ComandoCliente::TRANSITION_SCREEN:
				Logger::getInstance()->log(Debug, "Recibio un TRANSITION_SCREEN");
				enviandoPing = false;
				enPantallaDeTransicion = true;
				this->maquinaDeEstados->cambiarNivel();
				break;
			case ComandoCliente::RESULTADO_PING:
				Logger::getInstance()->log(Debug, "Recibio un RESULTADO_PING");
				enviandoPing = false;
				break;
			case ComandoCliente::RESULTADO_USUARIOS:
				Logger::getInstance()->log(Debug, "se recibio una lista de usuarios");
				//mostrarUsuariosConectados(mensajeDeRed);
				procesarNombresUsuario(mensajeDeRed);
				break;
			case ComandoCliente::GAME_OVER:
				Logger::getInstance()->log(Debug, "Recibio un GAME_OVER");
				dibujarGameOver = true;
				break;
			default:
				Logger::getInstance()->log(Debug, datosRecibidos);
			}
			if (datosRecibidos != NULL)
				free(datosRecibidos);
			if (mensajeDeRed != NULL)
				delete mensajeDeRed;
			if (estadoInicialJuego != NULL)
				delete estadoInicialJuego;
			if (estadoModeloJuego != NULL)
				delete estadoModeloJuego;

			timeOut = std::chrono::high_resolution_clock::now();
		} 


		double tiempoTardado = std::chrono::duration_cast<std::chrono::duration<double, std::ratio<1>>>(std::chrono::high_resolution_clock::now() - timeOut).count() * 1000;
		if (!enPantallaDeTransicion /*&& this->juegoIniciado*/ && tiempoTardado > (Constantes::PING_DELAY) + Constantes::TOLERANCIA_PING) {
			this->estaLogueado = false;
			this->conexionViva = false;
			this->clienteActivo = false;
			Logger::getInstance()->log(Debug, "Se desconecto un cliente del servidor por falta de respuesta al ping");
			std::cout << "Se ha desconectado del servidor" << std::endl;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1000/(Constantes::FPS - 7)));
		//mostrarMensajesGlobales();
	}
}

void Cliente::procesarNombresUsuario(MensajeDeRed* unMensajeDeRed) {
	// Asocia los ids de los usuarios con los nombres, recibe de a pares de parametros primero el id y luego el nombre
	m_init_juego.lock();
	for (unsigned int i = 0; i < unMensajeDeRed->getCantidadDeParametros(); i++) {
		this->nombreJugadores[atoi(unMensajeDeRed->getParametro(i).c_str())] = unMensajeDeRed->getParametro(i + 1);
		i++;
	}
	m_init_juego.unlock();
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

void Cliente::iniciarJuego(EstadoInicialJuego* unEstadoInicial) {
	if (!this->juegoIniciado) {
		/*EstadoInicialJuego* estadoInicial = new EstadoInicialJuego();
		estadoInicial->idJugador = atoi(mensajeInit->getParametro(0).c_str());
		estadoInicial->tamanio = atoi(mensajeInit->getParametro(1).c_str());

		std::string tmpParseString = mensajeInit->getParametro(2);

		for (int i = 0; i < estadoInicial->tamanio; i++) {
			int posSeparadorIds = tmpParseString.find_first_of(&Constantes::separadorIds);
			estadoInicial->id[i] = atoi(tmpParseString.substr(0, posSeparadorIds).c_str());
			tmpParseString.erase(0, posSeparadorIds + 1);
		}*/
		m_init_juego.lock();
		EstadoJuegoActivo* estadoJuegoActivo = new EstadoJuegoActivo();
		estadoJuegoActivo->setNombresJugadores(this->nombreJugadores);
		estadoJuegoActivo->setNivel(unEstadoInicial->nivel);
		this->maquinaDeEstados->changeState(estadoJuegoActivo, unEstadoInicial);
		this->juegoIniciado = true;
		m_init_juego.unlock();
	}
}

void Cliente::procesarEstadoModelo(EstadoModeloJuego* estadoModeloJuego) {
	if(juegoIniciado)
		this->maquinaDeEstados->recieveInput(estadoModeloJuego);
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

void Cliente::procesarResultadoLogin(MensajeDeRed* mensajeDeRed, char* datosRecibidos) {
	if (mensajeDeRed->getParametro(0) == "LOGIN_OK") {
		this->estaLogueado = true;
		if (mensajeDeRed->getParametro(1).compare("0") == 0) {
			this->idJugador = stoi(mensajeDeRed->getParametro(2));
			//this->maquinaDeEstados->changeState(new EstadoJuegoActivo());
			this->maquinaDeEstados->changeState(new EstadoEspera());
			m_print.lock();
			cout << mensajeDeRed->getParametro(2) << endl;
			m_print.unlock();
		} else {
			EstadoInicialJuego* estadoInicial = new EstadoInicialJuego();
			memcpy(estadoInicial, datosRecibidos + 27, sizeof(EstadoInicialJuego));
			iniciarJuego(estadoInicial);
		}
	} else if (mensajeDeRed->getParametro(0) == "LOGIN_NOK") {
		//esconectarseDelServidor();
		this->estaLogueado = false;
		string mensaje = "Error, reingrese usuario:";
		this->maquinaDeEstados->changeState(new EstadoLogeo(),&mensaje);
		m_print.lock();
		cout << mensajeDeRed->getParametro(1) << endl;
		m_print.unlock();
		
	}
}



void Cliente::enviarPingAServidor() {
	/*while (this->conexionViva) {
		if (this->juegoIniciado) {
			//Logger::getInstance()->log(Debug, "Cliente enviando PING al servidor");
			ManejadorInput::getInstance()->setCerrar(!this->conexionDelCliente->enviarSolicitudPing());
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}*/
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

