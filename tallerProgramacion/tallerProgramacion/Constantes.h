#pragma once
#ifndef CONSTANTES_H
#define CONSTANTES_H

#include <string>

using namespace std;

// Actualizar a medida que necesitemos
enum class ComandoServidor { LOG = '0', PING = '1', SEND_MESSAGE = '2', RETRIEVE_MESSAGES = '3', VACIO = '4', USUARIOS = '5', RESULTADO_PING = '6'};
enum class ComandoCliente { LOG = '0', PRINT = '1', VACIO = '2', RESULTADO_LOGIN = '3', RESULTADO_SEND_MESSAGE = '4', RESULTADO_RETRIEVE_MESSAGES = '5', PING = '6', RECIEVE_GLOBAL_MESSAGES = '7', RECIEVE_PRIVATE_MESSAGES = '8', RESULTADO_USUARIOS = '9', RESULTADO_PING
	= '10'};

class Constantes
{
public:
	static constexpr const char* CLIENTE = "CLIENTE";
	static constexpr const char* SERVIDOR = "SERVIDOR";
	static const char separador = (char)'/0';
	string getComandoServidor(ComandoServidor comando);
	string getComandoCliente(ComandoCliente);
	ComandoServidor getConstComandoServidor(string comando);
	ComandoCliente getConstComandoCliente(string comando);
	~Constantes();
	static Constantes* getInstance();
	static constexpr const int PING_DELAY = 100000; // grande por ahora para que no moleste
private:
	Constantes();
	static Constantes* instance;
};

#endif