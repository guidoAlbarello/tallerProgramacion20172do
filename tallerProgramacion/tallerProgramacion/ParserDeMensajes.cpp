#include "ParserDeMensajes.h"
#include <string>

using namespace std;
ParserDeMensajes* ParserDeMensajes::instance = NULL;

ParserDeMensajes::ParserDeMensajes()
{
}


ParserDeMensajes::~ParserDeMensajes()
{
}

ParserDeMensajes *ParserDeMensajes::getInstance() {
	if (!instance) {
		instance = new ParserDeMensajes();
	}

	return instance;
}

string ParserDeMensajes::getTipoDeAccion(string mensaje) {
	return NULL;
}

string ParserDeMensajes::getParametro(string mensaje, int indice) {
	return NULL;
}

int ParserDeMensajes::getCantidadDeParametros(string mensaje){
	return 0;
}

ComandoServidor ParserDeMensajes::getComandoServidor(string mensaje) {
	std::size_t found = mensaje.find_first_of(Constantes::getInstance()->separador);
	string comandoStr = mensaje.substr(0, found);

	return Constantes::getInstance()->getConstComandoServidor(comandoStr);
}

ComandoCliente ParserDeMensajes::getComandoCliente(string mensaje) {
	std::size_t found = mensaje.find_first_of(Constantes::getInstance()->separador);
	string comandoStr = mensaje.substr(0, found);

	return Constantes::getInstance()->getConstComandoCliente(comandoStr);
}
