#pragma once
#ifndef  SERVIDOR_DE_CHAT_H
#define SERVIDOR_DE_CHAT_H

#include "Servidor.h"
#include "Usuario.h"
#include "Conexion.h"
#include "Logger.h"
#include <iostream>
#include <vector>

class ServidorDeChat : public Servidor {
public:
	static ServidorDeChat* getInstance();
	Usuario* buscarUsuario(std::string unUsuario);
private:
	ServidorDeChat();
	static ServidorDeChat* instance;

	void cerrarTodasLasConexiones();
	void correrCicloPrincipal();
	void cambiarNivelLogeo();
	void mostrarUsuariosConectados();
	void mostrarMenu();
	std::vector<Usuario*> listaDeUsuarios;
	std::vector<Conexion*> conexionesActivas;
	void procesarDatosRecibidos();
};

#endif