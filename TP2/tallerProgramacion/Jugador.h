#pragma once
#ifndef JUGADOR_H
#define JUGADOR_H

#include "ObjetoDeJuego.h"
#include "Camara.h"
#define ACELERACION_AUTO_X 1.0
#define ACELERACION_AUTO_Y 1.5
#define FACTOR_DESACELERACION_X 0
#define FACTOR_DESACELERACION_Y 30.0
#define LIMITE_VELOCIDAD_AUTO_X 60.0
#define LIMITE_VELOCIDAD_AUTO_Y 120.0

class Jugador : public ObjetoDeJuego {  //tal vez diferenciar entre movil y estatico

public:
	Jugador();
	Jugador(SDL_Renderer* renderer);
	void update(Unidad delta);
	void recibirEntrada(int pos, bool estadoEntrada);
	void setId(int id) { this->id = id; }
	int getId() { return this->id; }
	void acelerar(Unidad delta);
	void doblarDerecha(Unidad delta);
	void doblarIzquierda(Unidad delta);
	void desacelerar(Unidad delta);
	void dejarDoblarDerecha(Unidad delta);
	void dejarDoblarIzquierda(Unidad delta);
	Unidad getPosicionX() { return posicion.getX(); }
	Unidad getPosicionY() { return posicion.getY(); }
	EstadoAuto getEstado() { return estado; }
	bool estaConectado() { return conectado; }
	void setEstadoConexion(bool estaConectado) { this->conectado = estaConectado; }
	Camara* getCamara() { return this->camara; }
protected:
	bool entrada[Constantes::CANT_TECLAS];
	int id;
	bool conectado = false;
	bool acelerando = false;
	EstadoAuto estado = EstadoAuto::DERECHO;
	Camara* camara;
};
#endif