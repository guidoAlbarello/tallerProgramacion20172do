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

#include <mutex>


#define MAX_SKIP_FRAMES 5
#define FPS Constantes::FPS

class Juego {
public:
	Juego();
	bool iniciarJuego();
	void update(Unidad tiempoDelta);
	void obtenerEntrada();
	void agregarObjetoDeJuego(ObjetoDeJuego* objetoDeJuego);
	std::vector<ObjetoDeJuego*> getObjetosDeJuego();
	std::vector<Jugador*> getJugadores();
	EstadoModeloJuego* getEstadoJuego();
	Jugador* agregarJugador();
	void liberarModeloEstado(EstadoModeloJuego* unEstado);
	bool jugadoresCargados() { return jugadores.size() == Constantes::CANT_JUGADORES_INICIAR; }
	EstadoInicialJuego* getEstadoJuegoInicial();
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
};

#endif