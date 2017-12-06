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
	case ComandoServidor::HPING:
		return "HPING";
	case ComandoServidor::SEND_MESSAGE:
		return "SEND_MESSAGE";
	case ComandoServidor::RETRIEVE_MESSAGES:
		return "RETRIEVE_MESSAGES";
	case ComandoServidor::UPDATE_MODEL:
		return "UPDATE_MODEL";
	case ComandoServidor::USUARIOS:
		return "USUARIOS";
	case ComandoServidor::INPUT:
		return "INPUT";
	case ComandoServidor::CLOSE:
		return "CLOSE"; 
	default:
		return "Not recognized..";
	}
}

ComandoServidor Constantes::getConstComandoServidor(string comando){
	if (comando.compare("LOG") == 0)
	{
		return ComandoServidor::LOG;
	}
	if (comando.compare("HPING") == 0)
	{
		return ComandoServidor::HPING;
	}
	if (comando.compare("SEND_MESSAGE") == 0)
	{
		return ComandoServidor::SEND_MESSAGE;
	}
	if (comando.compare("RETRIEVE_MESSAGES") == 0)
	{
		return ComandoServidor::RETRIEVE_MESSAGES;
	}
	if (comando.compare("UPDATE_MODEL") == 0)
	{
		return ComandoServidor::UPDATE_MODEL;
	}
	if (comando.compare("USUARIOS") == 0)
	{
		return ComandoServidor::USUARIOS;
	}
	if (comando.compare("INPUT") == 0) {
		return ComandoServidor::INPUT;
	}
	if (comando.compare("CLOSE") == 0) {
		return ComandoServidor::CLOSE;
	}
	
	return ComandoServidor::UPDATE_MODEL;
}

string Constantes::getComandoCliente(ComandoCliente comando)
{
	switch (comando)
	{
	case ComandoCliente::LOG:
		return "LOG";
	case ComandoCliente::PRINT:
		return "PRINT";
	case ComandoCliente::UPDATE_MODEL:
		return "UPDATE_MODEL";
	case ComandoCliente::RESULTADO_LOGIN:
		return "RESULTADO_LOGIN";
	case ComandoCliente::RESULTADO_SEND_MESSAGE:
		return "RESULTADO_SEND_MESSAGE";
	case ComandoCliente::RESULTADO_RETRIEVE_MESSAGES:
		return "RESULTADO_RETRIEVE_MESSAGES";
	case ComandoCliente::RECIEVE_GLOBAL_MESSAGES:
		return "RECIEVE_GLOBAL_MESSAGES";
	case ComandoCliente::RECIEVE_PRIVATE_MESSAGES:
		return "RECIEVE_PRIVATE_MESSAGES";
	case ComandoCliente::HRESULTADO_PING:
		return "HRESULTADO_PING";
	case ComandoCliente::RESULTADO_USUARIOS:
		return "RESULTADO_USUARIOS";
	case ComandoCliente::INIT:
		return "INIT";
	case ComandoCliente::TRANSITION_SCREEN:
		return "TRANSITION_SCREEN";
	case ComandoCliente::GAME_OVER:
		return "GAME_OVER"; 
	case ComandoCliente::CLOSE:
		return "CLOSE"; 
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

	if (comando.compare("RECIEVE_GLOBAL_MESSAGES") == 0) {
		return ComandoCliente::RECIEVE_GLOBAL_MESSAGES;
	}

	if (comando.compare("RECIEVE_PRIVATE_MESSAGES") == 0) {
		return ComandoCliente::RECIEVE_PRIVATE_MESSAGES;
	}

	if (comando.compare("HRESULTADO_PING") == 0) {
		return ComandoCliente::HRESULTADO_PING;
	}

	if (comando.compare("RESULTADO_USUARIOS") == 0) {
		return ComandoCliente::RESULTADO_USUARIOS;
	}

	if (comando.compare("INIT") == 0) {
		return ComandoCliente::INIT;
	}

	if (comando.compare("TRANSITION_SCREEN") == 0) {
		return ComandoCliente::TRANSITION_SCREEN;
	}

	if (comando.compare("GAME_OVER") == 0) {
		return ComandoCliente::GAME_OVER;
	}
	if (comando.compare("CLOSE") == 0) {
		return ComandoCliente::CLOSE;
	}
	
	return ComandoCliente::UPDATE_MODEL;
}


Constantes *Constantes::getInstance() {
	if (!instance) {
		instance = new Constantes();
	}

	return instance;
}