#pragma once
#ifndef  MANEJADOR_DE_CONEXION_CONEXION_H
#define MANEJADOR_DE_CONEXION_CONEXION_H
#include "ManejadorDeConexion.h"


class ManejadorDeConexionConexion : public ManejadorDeConexion {
public:
	ManejadorDeConexionConexion(SOCKET unSocket);

	void iniciarConexion();
protected:
};

#endif