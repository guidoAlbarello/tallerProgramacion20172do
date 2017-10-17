#pragma once
#ifndef  MANEJADOR_DE_CONEXION_SERVIDOR_H
#define MANEJADOR_DE_CONEXION_SERVIDOR_H

#include "ManejadorDeConexion.h"
#include <vector>

class ManejadorDeConexionServidor : public ManejadorDeConexion {
public:

	void iniciarConexion(std::string puertoEscucha, int cantidadConexionesMaxima);
	SOCKET hayClienteIntentandoConectarse(size_t conexionesActivas, int maxClientes);
	void borrarEntorno();
protected:
	std::thread t_escucharClientes;

};

#endif