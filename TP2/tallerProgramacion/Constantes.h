#pragma once
#ifndef CONSTANTES_H
#define CONSTANTES_H

#include <string>

using namespace std;

// Actualizar a medida que necesitemos
enum class ComandoServidor { LOG = '0', PING = '1', SEND_MESSAGE = '2', RETRIEVE_MESSAGES = '3', VACIO = '4', USUARIOS = '5', INPUT = '6'};
enum class ComandoCliente { LOG = '0', PRINT = '1', VACIO = '2', RESULTADO_LOGIN = '3', RESULTADO_SEND_MESSAGE = '4',
	RESULTADO_RETRIEVE_MESSAGES = '5', RECIEVE_GLOBAL_MESSAGES = '6', RECIEVE_PRIVATE_MESSAGES = '7', 
	RESULTADO_USUARIOS = '8', RESULTADO_PING	= '9'};

typedef float Unidad;

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
	static constexpr const int PING_DELAY = 1000;
	static constexpr const int RECV_TIMEOUT = 3000;  //3 segundos
private:
	Constantes();
	static Constantes* instance;
};

#endif