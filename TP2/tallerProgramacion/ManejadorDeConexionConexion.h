#pragma once
#ifndef  MANEJADOR_DE_CONEXION_CONEXION_H
#define MANEJADOR_DE_CONEXION_CONEXION_H
#include "ManejadorDeConexion.h"
#include "MensajeDeRed.h"
#include "Logger.h"

class ManejadorDeConexionConexion : public ManejadorDeConexion {
public:
	ManejadorDeConexionConexion(SOCKET unSocket);
	bool cerrarCliente();
	void iniciarConexion();
protected:
};

#endif