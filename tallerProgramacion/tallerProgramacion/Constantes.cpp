#include "Constantes.h"

Constantes* Constantes::instance = NULL;

Constantes::Constantes()
{
}

Constantes::~Constantes()
{
}

string Constantes::getComandoServidor(ComandoServidor comando)
{
	switch (comando)
	{
	case ComandoServidor::LOG:
		return "LOG";
	case ComandoServidor::PING:
		return "PING";
	case ComandoServidor::SEND_MESSAGE:
		return "SEND_MESSAGE";
	case ComandoServidor::RETRIEVE_MESSAGES:
		return "RETRIEVE_MESSAGES";
	default:
		return "Not recognized..";
	}
}

ComandoServidor Constantes::getConstComandoServidor(string comando){
	if (comando.compare("LOG") == 0)
	{
		return ComandoServidor::LOG;
	}
	if (comando.compare("PING") == 0)
	{
		return ComandoServidor::PING;
	}
	if (comando.compare("SEND_MESSAGE") == 0)
	{
		return ComandoServidor::SEND_MESSAGE;
	}
	if (comando.compare("RETRIEVE_MESSAGES") == 0)
	{
		return ComandoServidor::RETRIEVE_MESSAGES;
	}
	
	return ComandoServidor::VACIO;
}

string Constantes::getComandoCliente(ComandoCliente comando)
{
	switch (comando)
	{
	case ComandoCliente::LOG:
		return "LOG";
	case ComandoCliente::PRINT:
		return "PRINT";
	case ComandoCliente::VACIO:
		return "SEND_MESSAGE";
	case ComandoCliente::RESULTADO_LOGIN:
		return "RESULTADO_LOGIN";
	default:
		return "Not recognized..";
	}
}

ComandoCliente Constantes::getConstComandoCliente(string comando) {
	if (comando.compare("LOG") == 0)
	{
		return ComandoCliente::LOG;
	}
	if (comando.compare("PRINT") == 0)
	{
		return ComandoCliente::PRINT;
	}
	if (comando.compare("RESULTADO_LOGIN") == 0)
	{
		return ComandoCliente::RESULTADO_LOGIN;
	}

	return ComandoCliente::VACIO;
}


Constantes *Constantes::getInstance() {
	if (!instance) {
		instance = new Constantes();
	}

	return instance;
}