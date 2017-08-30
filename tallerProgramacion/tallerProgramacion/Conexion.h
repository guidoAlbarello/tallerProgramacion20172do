#ifndef CONEXION_H
#define CONEXION_H

#include "Usuario.h"

class Conexion {
public:
	void cerrarConexion();
	Usuario* getUsuario() { return usuarioConectado; }
private:
	Usuario* usuarioConectado;
};

#endif