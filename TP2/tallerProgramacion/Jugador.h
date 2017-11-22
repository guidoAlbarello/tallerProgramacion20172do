#pragma once
#ifndef JUGADOR_H
#define JUGADOR_H

#include "ObjetoDeJuego.h"
#include "Camara.h"
#include <chrono>

#define ACELERACION_AUTO_X 1.0
#define ACELERACION_AUTO_Y 0.5
#define FACTOR_DESACELERACION_X 0
#define FACTOR_DESACELERACION_Y 0.09
#define LIMITE_VELOCIDAD_AUTO_X 60.0
#define LIMITE_VELOCIDAD_AUTO_Y_PASTO 40.0
#define LIMITE_VELOCIDAD_AUTO_Y_PISTA 120.0
#define ACELERACION_NITRO 3.0

class Jugador : public ObjetoDeJuego {  //tal vez diferenciar entre movil y estatico

public:
	Jugador(string nombre);
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
	void setPosicionY(Unidad y) { this->posicion.setY(y); }
	void setPosicionX(Unidad x) { this->posicion.setX(x); }
	EstadoAuto getEstado() { return estado; }
	bool estaConectado() { return conectado; }
	void setEstadoConexion(bool estaConectado) { this->conectado = estaConectado; }
	Camara* getCamara() { return this->camara; }
	bool getAcelerando() { return this->acelerando; }
	bool estaChocado();
	int getVida() { return vida; }
	void chocar(double posicionY);
	void setDeshabilitarMovimiento(bool estado) { movimientoDeshabilitado = estado; }
	bool getDeshabilitarMovimiento() { return movimientoDeshabilitado; }
	long getPuntos();
	void addPuntos(long agregar);
	string getNombre() { return this->nombre; }
	long getTiempo();
	void setTiempo(long t);
protected:
	void dañarAuto();
	bool entrada[Constantes::CANT_TECLAS];
	int id;
	bool conectado = false;
	bool acelerando = false;
	EstadoAuto estado = EstadoAuto::DERECHO;
	Camara* camara;
	float velocidadMaxima;
	bool chocado;
	bool movimientoDeshabilitado = false;
	long puntos;
	bool usarNitro = false;
	int vida = 3;
	double tiempoNitro = 0;
	string nombre;
	std::chrono::time_point<std::chrono::steady_clock> inicioIntervalo;
	long tiempo;
};
#endif