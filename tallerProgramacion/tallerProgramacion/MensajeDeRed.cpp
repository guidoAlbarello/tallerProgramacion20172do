#include "MensajeDeRed.h"
#include "ParserDeMensajes.h"


MensajeDeRed::MensajeDeRed(string mensaje)
{
	this->comando = ParserDeMensajes::getInstance()->getComando(mensaje);
	int current = mensaje.find_first_of(Constantes::getInstance()->separador);
	//Borro el tipo de mensaje
	mensaje.erase(0, current + 1);
	int i = 0;
	while (mensaje.length() > 0) {
		current = mensaje.find_first_of(Constantes::getInstance()->separador);
		if (current >= 0) {
			parametros.push_back( mensaje.substr(0, current));
			mensaje.erase(0, current + 1);
		}
		else {
			//era el ultimo
			parametros.push_back( mensaje );
			mensaje.erase(0, mensaje.length());

		}
		i++;
	}

}


MensajeDeRed::~MensajeDeRed()
{

}

Comando MensajeDeRed::getComando()
{
	return this->comando;
}

int MensajeDeRed::getCantidadDeParametros()
{
	return parametros.size();
}

string MensajeDeRed::getParametro(int i)
{
	return "bla";
}
