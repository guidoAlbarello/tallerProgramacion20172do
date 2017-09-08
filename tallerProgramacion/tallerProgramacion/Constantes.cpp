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

Constantes *Constantes::getInstance() {
	if (!instance) {
		instance = new Constantes();
	}

	return instance;
}