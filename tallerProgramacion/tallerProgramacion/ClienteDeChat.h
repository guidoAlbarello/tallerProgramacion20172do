#pragma once
#ifndef  CLIENTE_DE_CHAT_H
#define CLIENTE_DE_CHAT_H

#include "Cliente.h"
#include <iostream>

class ClienteDeChat : public Cliente {
public:
	static ClienteDeChat* getInstance();
	
private:
	ClienteDeChat();
	static ClienteDeChat* instance;
	void mostrarMenu();
	void correrCicloPrincipal();
	void conectarseAlServidor();
	void desconectarseDelServidor();
	void hacerTestDeEstres();
	void revisarBuzon();
	void logearseAlServidor();
	void enviarMensajeAlChat();
	void enviarMensajePrivado();
};

#endif