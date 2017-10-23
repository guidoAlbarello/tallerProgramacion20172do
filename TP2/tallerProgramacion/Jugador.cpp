#include "Jugador.h"

Jugador::Jugador() {

}

void Jugador::recibirEntrada(int pos, bool estadoEntrada) {
	entrada[pos] = estadoEntrada;
}

