#include "Jugador.h"

Jugador::Jugador(SDL_Renderer* renderer): ObjetoDeJuego(renderer) {
	//se llamo automaticamente al constructor de la superclase
}
void Jugador::update(Unidad delta) {
	/*if(entrada[0])			//tecla arriba

	if(entrada[1])			//tecla derecha

	if(entrada[2])			//tecla izquierda*/
}
void Jugador::recibirEntrada(int pos, bool estadoEntrada) {
	entrada[pos] = estadoEntrada;
}

