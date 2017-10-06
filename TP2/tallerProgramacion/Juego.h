#pragma once
#include "Constantes.h"
#include <thread>
#include <chrono>
#include <vector>
#include "Jugador.h"

#include <mutex>


#define MAX_SKIP_FRAMES 5
#define FPS 25

class Juego {
public:
	Juego();
	bool iniciarJuego();
	void update(Unidad tiempoDelta);
	void obtenerEntrada();
	std::vector<ObjetoDeJuego*> getObjetosDeJuego();
	std::vector<Jugador*> getJugadores();
	Jugador* agregarJugador();
private:
	int cantidadJugadores;
	bool juegoActivo;
	void gameLoop();
	thread t_gameLoop;
	std::vector<ObjetoDeJuego*> objetosDeJuego;
	std::vector<Jugador*> jugadores;
	mutex m_jugadores;
	mutex m_objetos;
};