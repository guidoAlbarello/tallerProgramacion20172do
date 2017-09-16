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
	case ComandoCliente::RESULTADO_SEND_MESSAGE:
		return "RESULTADO_SEND_MESSAGE";
	case ComandoCliente::RESULTADO_RETRIEVE_MESSAGES:
		return "RESULTADO_RETRIEVE_MESSAGES";
	case ComandoCliente::PING:
		return "PING";
	case ComandoCliente::RECIEVE_GLOBAL_MESSAGES:
		return "RECIEVE_GLOBAL_MESSAGES";
	case ComandoCliente::RECIEVE_PRIVATE_MESSAGES:
		return "RECIEVE_PRIVATE_MESSAGES";
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

	if (comando.compare("RESULTADO_SEND_MESSAGE") == 0) {
		return ComandoCliente::RESULTADO_SEND_MESSAGE;
	}

	if (comando.compare("RESULTADO_RETRIEVE_MESSAGES") == 0) {
		return ComandoCliente::RESULTADO_RETRIEVE_MESSAGES;
	}

	if (comando.compare("PING") == 0) {
		return ComandoCliente::PING;
	}

	if (comando.compare("RECIEVE_GLOBAL_MESSAGES") == 0) {
		return ComandoCliente::RECIEVE_GLOBAL_MESSAGES;
	}

	if (comando.compare("RECIEVE_PRIVATE_MESSAGES") == 0) {
		return ComandoCliente::RECIEVE_PRIVATE_MESSAGES;
	}

	return ComandoCliente::VACIO;
}


Constantes *Constantes::getInstance() {
	if (!instance) {
		instance = new Constantes();
	}

	return instance;
}