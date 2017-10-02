#include "MensajeDeRed.h"
#include "ParserDeMensajes.h"


MensajeDeRed::MensajeDeRed(string mensaje, string destinatario)
{
	if (Constantes::CLIENTE == destinatario) {
		this->comandoCliente = ParserDeMensajes::getInstance()->getComandoCliente(mensaje);
	}
	else if (Constantes::SERVIDOR == destinatario) {
		this->comandoServidor = ParserDeMensajes::getInstance()->getComandoServidor(mensaje);
	}
	int current = mensaje.find_first_of(Constantes::getInstance()->separador);
	//Borro el tipo de mensaje
	mensaje.erase(0, current + 1);
	int i = 0;
	while (mensaje.length()   > 0) {
		current = mensaje.find_first_of(Constantes::getInstance()->separador);
		if (current >= 0) {
			parametros.push_back(mensaje.substr(0, current));
			mensaje.erase(0, current + 1);
		}
		else {
			//era el ultimo
			parametros.push_back(mensaje);
			mensaje.erase(0, mensaje.length() +1 );

		}
		i++;
	}
}

MensajeDeRed::MensajeDeRed(ComandoServidor comando)
{
	this->comandoServidor = comando;
}

MensajeDeRed::MensajeDeRed(ComandoCliente comando)
{
	this->comandoCliente = comando;
}

void MensajeDeRed::agregarParametro(string parametro) {
	parametros.push_back(parametro);
}

string MensajeDeRed::getComandoServidorSerializado() {
	string result = "";
	result = Constantes::getInstance()->getComandoServidor(this->getComandoServidor());
	for (unsigned int i = 0; i < parametros.size(); i++) {
		result += Constantes::getInstance()->separador;
		result += parametros.at(i);
	}
	return result;
}

string MensajeDeRed::getComandoClienteSerializado() {
	string result = "";
	result = Constantes::getInstance()->getComandoCliente(this->getComandoCliente());
	for (unsigned int i = 0; i < parametros.size(); i++) {
		result += Constantes::getInstance()->separador;
		result += parametros.at(i);
	}
	return result;
}

MensajeDeRed::~MensajeDeRed()
{

}

ComandoServidor MensajeDeRed::getComandoServidor()
{
	return this->comandoServidor;
}

ComandoCliente MensajeDeRed::getComandoCliente() {
	return this->comandoCliente;
}

int MensajeDeRed::getCantidadDeParametros()
{
	return parametros.size();
}

string MensajeDeRed::getParametro(int i)
{
	return parametros.at(i);
}
