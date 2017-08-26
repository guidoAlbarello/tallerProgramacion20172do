#pragma once
#ifndef  SERVIDOR_DE_CORREO_H
#define SERVIDOR_DE_CORREO_H

#include "Servidor.h"
#include <iostream>
class ServidorDeCorreo : public Servidor {
public:
	ServidorDeCorreo();
	
private:
	void cerrarTodasLasConexiones();
	void correrCicloPrincipal();
	void cambiarNivelLogeo();
	void mostrarUsuarioConectados();
	void mostrarMenu();
};

#endif