#pragma once
#ifndef  MANEJADOR_DE_CONEXION_CLIENTE_H
#define MANEJADOR_DE_CONEXION__CLIENTE_H


#include "ManejadorDeConexion.h"
#include "ManejadorInput.h"

using namespace std;

class ManejadorDeConexionCliente : public ManejadorDeConexion {
public:

	bool enviarEntrada();
	bool iniciarConexion(string ipServidor, string puertoServidor);
	//bool ejecutarComando(Comando comando, std::stri, std::string puertoServidor);
	bool login(string user, string pass);
	bool enviarMensajeGlobal(string unMensaje);
	bool desconectarServidor();
	bool enviarMensajePrivado(string unDestinatario, string unMensaje);
	bool realizarPeticionUsuarios();
	bool devolverMensajesPrivados();
	void borrarEntorno();

	bool enviarSolicitudPing();


};

#endif