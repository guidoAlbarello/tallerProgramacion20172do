#pragma once
#ifndef CONSTANTES_H
#define CONSTANTES_H

#include <string>

using namespace std;
enum Comando { LOG = '0', PING = '1', SEND_MESSAGE = '2', RETRIEVE_MESSAGES = '3', VACIO = '4' };
class Constantes
{
public:
	static const char separador = (char)'/0';
	string getComando(Comando comando);
	Comando getConstComando(string comando);
	~Constantes();
	static Constantes* getInstance();
private:
	Constantes();
	static Constantes* instance;
};

#endif