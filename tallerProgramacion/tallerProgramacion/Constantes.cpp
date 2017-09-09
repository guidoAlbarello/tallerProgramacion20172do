#include "Constantes.h"



Constantes* Constantes::instance = NULL;


Constantes::Constantes()
{
}


Constantes::~Constantes()
{
}

string Constantes::getComando(Comando comando)
{
	switch (comando)
	{
	case Comando::LOG:
		return "LOG";
	case Comando::PING:
		return "PING";
	case Comando::SEND_MESSAGE:
		return "SEND_MESSAGE";
	case Comando::RETRIEVE_MESSAGES:
		return "RETRIEVE_MESSAGES";
	default:
		return "Not recognized..";
	}
}

Comando Constantes::getConstComando(string comando){
	if (comando.compare("LOG") == 0)
	{
		return LOG;
	}
	if (comando.compare("PING") == 0)
	{
		return PING;
	}
	if (comando.compare("SEND_MESSAGE") == 0)
	{
		return SEND_MESSAGE;
	}
	if (comando.compare("RETRIEVE_MESSAGES") == 0)
	{
		return RETRIEVE_MESSAGES;
	}
	
	return VACIO;
}

Constantes *Constantes::getInstance() {
	if (!instance) {
		instance = new Constantes();
	}

	return instance;
}