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
#include "MaquinaEstados.h"
#include "Cartel.h"


#define MAX_SKIP_FRAMES 5
#define FACTOR_CHOQUE 1
// ESTO ES... ALTO 200 / ALTO_TRAMO
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
	Jugador* agregarJugador(string nombre);
	void liberarModeloEstado(EstadoModeloJuego* unEstado);
	bool jugadoresCargados() { return jugadores.size() == cantidadJugadoresMaxima; }
	EstadoInicialJuego* getEstadoJuegoInicial();
	void setMapa(int i,Mapa* mapa) { this->mapa[i] = mapa; }
	Mapa* getMapa(int i) { return this->mapa[i]; }
	void procesarMapa();
	int hayColision(int yDesde, int yHasta, int xDesde, int xHasta, Jugador* objeto2);
	int hayColisionObjetoFijo(int yDesde, int yHasta, int xDesde, int xHasta, ObjetoFijo* objeto2);
	void inicializarNivel();
	bool terminoNivel();
	int getNivel() { return nivel; }
	bool gameOver() { return juegoTerminado; }
private:
	Renderer* renderer;
	int cantidadJugadores = 0;
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
	Mapa* mapa[3];
	std::vector<Segmento*> segmentos;
	bool Juego::vaPuntero(Jugador * unJugador);
	int nivel = 0;
	bool juegoTerminado = false;
	unsigned long tiempo;
};

#endif