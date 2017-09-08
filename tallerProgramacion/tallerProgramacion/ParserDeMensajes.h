#pragma once

#ifndef  PARSERDEMENSAJES_H
#define  PARSERDEMENSAJES_H

#include <string>

using namespace std;

class ParserDeMensajes
{
public:
	static ParserDeMensajes* getInstance();
	~ParserDeMensajes();
	string getTipoDeAccion(string mensaje);
	string getParametro(string mensaje, int indice);
	int getCantidadDeParametros(string mensaje);

private:
	ParserDeMensajes();
	static ParserDeMensajes* instance;
};

#endif

