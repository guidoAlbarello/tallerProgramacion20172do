#pragma once

#ifndef JUEGO_H
#define JUEGO_H
#include "Constantes.h"
#include <thread>
#include <chrono>
#include <vector>
#include "Jugador.h"
#include "Escenario.h"
#include "Renderer.h"
#include "Mapa.h"
#include <mutex>


#define MAX_SKIP_FRAMES 5

class Juego {
public:
	Juego();
	bool iniciarJuego(int cantidadJugadoresMaxima);
	void update(Unidad tiempoDelta);
	void obtenerEntrada();
	void agregarObjetoDeJuego(ObjetoDeJuego* objetoDeJuego);
	std::vector<ObjetoDeJuego*> getObjetosDeJuego();
	std::vector<Jugador*> getJugadores();
	EstadoModeloJuego* getEstadoJuego();
	Jugador* agregarJugador();
	void liberarModeloEstado(EstadoModeloJuego* unEstado);
	bool jugadoresCargados() { return jugadores.size() == cantidadJugadoresMaxima; }
	EstadoInicialJuego* getEstadoJuegoInicial();
	void setMapa(Mapa* mapa) { this->mapa = mapa; }
	Mapa* getMapa() { return this->mapa; }
	void procesarMapa();
	int hayColision(int yDesde, int yHasta, int xDesde, int xHasta, ObjetoDeJuego* objeto2);
	int hayColisionObjetoFijo(int yDesde, int yHasta, int xDesde, int xHasta, ObjetoFijo* objeto2);
private:
	Renderer* renderer;
	int cantidadJugadores;
	bool juegoActivo;
	void iniciarEscenario();
	void gameLoop();
	thread t_gameLoop;
	std::vector<ObjetoDeJuego*> objetosDeJuego;
	std::vector<Jugador*> jugadores;
	mutex m_jugadores;
	mutex m_objetos;
	Escenario* escenario;
	int cantidadJugadoresMaxima; 
	Mapa* mapa;
	std::vector<Segmento*> segmentos;
	bool Juego::vaPuntero(Jugador * unJugador);
};

#endif