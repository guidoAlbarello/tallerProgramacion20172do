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
#define FPS 25

class Juego {
public:
	Juego();
	Juego(Renderer *renderer);
	bool iniciarJuego();
	void update(Unidad tiempoDelta);
	void obtenerEntrada();
	void agregarObjetoDeJuego(ObjetoDeJuego* objetoDeJuego);
	std::vector<ObjetoDeJuego*> getObjetosDeJuego();
	std::vector<Jugador*> getJugadores();
	Jugador* agregarJugador();
	Escenario * escenario;
private:
	Renderer* renderer;
	int cantidadJugadores;
	bool juegoActivo;
	void iniciarEscenario();
	void gameLoop();
	thread t_gameLoop;
	std::vector<ObjetoDeJuego*> objetosDeJuego;
	std::vector<Jugador*> jugadores;
	std::vector<SDL_Keycode> entradas;
	mutex m_jugadores;
	mutex m_objetos;
};

#endif