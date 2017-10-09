#pragma once
#ifndef USUARIO_H
#define USUARIO_H
#include <iostream>
#include "BuzonDeMensajes.h"
#include "Jugador.h"

class Usuario {
public:
	Usuario();
	Usuario(std::string nombre, std::string password);
	std::string getNombre();
	void setNombre(std::string nombre);
	std::string getPassword();
	void setPassword(std::string password);
	void enviarMensaje(Usuario* unUsuario, std::string unMensaje);
	void recibirMensaje(std::string unDestinatario, std::string unEmisor, std::string unMensaje);
	BuzonDeMensajes* getBuzon() { return buzonDeMensajes; }
	Jugador* getJugador() { return jugador; }
	void setJugador(Jugador* unJugador) { this->jugador = unJugador; }
	void setId(int id) { this->jugador->setId(id); }
	int getId() { return this->jugador->getId(); }
private:
	std::string nombre;
	std::string password;
	BuzonDeMensajes* buzonDeMensajes;
	Jugador* jugador;

};

#endif