#include "Jugador.h"

Jugador::Jugador(SDL_Renderer* renderer): ObjetoDeJuego(renderer) {
	//se llamo automaticamente al constructor de la superclase
}
void Jugador::recibirEntrada(int pos, bool estadoEntrada) {
	entrada[pos] = estadoEntrada;
}

