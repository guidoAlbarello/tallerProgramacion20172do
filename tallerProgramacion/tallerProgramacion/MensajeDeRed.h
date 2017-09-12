#pragma once
#ifndef MENSAJESDERED_H
#define MENSAJESDERED_H

#include "Constantes.h"
#include <string>
#include <vector>

using namespace std;

class MensajeDeRed
{
public:
	MensajeDeRed(string mensaje);
	MensajeDeRed(Comando comando);

	~MensajeDeRed();
	Comando getComando();
	int getCantidadDeParametros();
	string getParametro(int i);
	void agregarParametro(string parametro);
	string getComandoSerializado();


private:
		Comando comando;
		vector<string> parametros;
};

#endif

