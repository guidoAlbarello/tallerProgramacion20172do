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
	MensajeDeRed(string mensaje, string destinatario);
	MensajeDeRed(ComandoServidor comando);
	MensajeDeRed(ComandoCliente comando);
	~MensajeDeRed();
	ComandoServidor getComandoServidor();
	ComandoCliente getComandoCliente();
	int getCantidadDeParametros();
	string getParametro(int i);
	void agregarParametro(string parametro);
	string getComandoServidorSerializado();
	string getComandoClienteSerializado();

private:
	ComandoServidor comandoServidor;
	ComandoCliente comandoCliente;
	vector<string> parametros;
};

#endif

