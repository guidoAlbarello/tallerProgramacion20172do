#pragma once
#include "Constantes.h"
#include <thread>
#include <chrono>
#include <vector>
#include "Jugador.h"

#define MAX_SKIP_FRAMES 5
#define FPS 25
class Juego {
public:
	bool iniciarJuego();
	void update(Unidad tiempoDelta);
	void obtenerEntrada();
	Juego();
private:
	bool juegoActivo;
	void gameLoop();
	thread t_gameLoop;
	std::vector<ObjetoDeJuego*> objetosDeJuego;
	std::vector<Jugador*> jugadores;
};