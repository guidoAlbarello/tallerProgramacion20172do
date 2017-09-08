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
