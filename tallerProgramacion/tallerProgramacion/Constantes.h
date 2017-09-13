#pragma once
#ifndef CONSTANTES_H
#define CONSTANTES_H

#include <string>

using namespace std;

// Actualizar a medida que necesitemos
enum class ComandoServidor { LOG = '0', PING = '1', SEND_MESSAGE = '2', RETRIEVE_MESSAGES = '3', VACIO = '4' };
enum class ComandoCliente { LOG = '0', PRINT = '1', VACIO = '2', RESULTADO_LOGIN = '3'};

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
private:
	Constantes();
	static Constantes* instance;
};

#endif