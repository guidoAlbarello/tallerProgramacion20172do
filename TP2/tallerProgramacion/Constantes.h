#pragma once
#ifndef CONSTANTES_H
#define CONSTANTES_H

#include <string>
#include  <SDL.h>

#define TECLA_DERECHA SDL_SCANCODE_RIGHT
#define TECLA_IZQUIERDA SDL_SCANCODE_LEFT
#define TECLA_ACELERAR SDL_SCANCODE_UP

using namespace std;

typedef float Unidad;
// Actualizar a medida que necesitemos
enum class ComandoServidor { LOG = '0', PING = '1', SEND_MESSAGE = '2', RETRIEVE_MESSAGES = '3', UPDATE_MODEL = '4', USUARIOS = '5', INPUT = '6'};
enum class ComandoCliente { LOG = '0', PRINT = '1', UPDATE_MODEL = '2', RESULTADO_LOGIN = '3', RESULTADO_SEND_MESSAGE = '4', RESULTADO_RETRIEVE_MESSAGES = '5', RECIEVE_GLOBAL_MESSAGES = '6', RECIEVE_PRIVATE_MESSAGES = '7', RESULTADO_USUARIOS = '8', RESULTADO_PING = '9', INIT = '10'};
enum class EstadoAuto { DERECHO = '0', DOBLANDO_IZQ = '1', DOBLANDO_DER = '2' };


class Constantes
{
public:
	static constexpr const char* CLIENTE = "CLIENTE";
	static constexpr const char* SERVIDOR = "SERVIDOR";
	static constexpr const char separador = char(6);
	string getComandoServidor(ComandoServidor comando);
	string getComandoCliente(ComandoCliente);
	ComandoServidor getConstComandoServidor(string comando);
	ComandoCliente getConstComandoCliente(string comando);
	~Constantes();
	static Constantes* getInstance();
	static constexpr const int PING_DELAY = 10000;
	static constexpr const int RECV_TIMEOUT = 25000;
	static constexpr const int FPS = 25;
	static constexpr const int UPDATE_MODEL_DELAY = 1000/Constantes::FPS;
	static constexpr const int MAX_CLIENTES = 8;
	static constexpr const int CANT_JUGADORES_INICIAR = 1;
	static constexpr const int CANT_TECLAS = 3;

private:
	Constantes();
	static Constantes* instance;
};

struct EstadoJugador {
	int id;
	Unidad posXCamara;
	Unidad posYCamara;
	Unidad posX;
	Unidad posY;
	EstadoAuto estadoAuto;
	bool conectado;
};

struct EstadoEscenario {
	Unidad cieloX;
	Unidad cieloY;
	Unidad colinasX;
	Unidad colinasY;
};

struct EstadoInicialJuego {
	/*EstadoJugador estados[Constantes::MAX_CLIENTES];*/
	//esto hay q ahcerlo dinamico una vez q tengamos objetos dinamicos
	int id[4];
	int tamanio;
	int idJugador;
};

struct EstadoModeloJuego {
	/*EstadoJugador estados[Constantes::MAX_CLIENTES];*/
	EstadoJugador estadoJugadores[4];
	EstadoEscenario estadoEscenario;
	int tamanio;
};
#endif