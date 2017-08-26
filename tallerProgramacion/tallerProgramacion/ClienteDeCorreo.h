#pragma once
#ifndef  CLIENTE_DE_CORREO_H
#define CLIENTE_DE_CORREO_H

#include "Cliente.h"

class ClienteDeCorreo : public Cliente {
public:
	ClienteDeCorreo();
	
private:
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