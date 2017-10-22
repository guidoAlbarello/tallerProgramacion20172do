#include "Jugador.h"

Jugador::Jugador(SDL_Renderer* renderer) : ObjetoDeJuego(renderer) {
	//se llamo automaticamente al constructor de la superclase
}
void Jugador::update(Unidad delta) {
	if (entrada[0]) 		//tecla arriba
		acelerar(delta);
	else
		desacelerar(delta);

	if (entrada[1])			//tecla derecha
		doblarDerecha(delta);
	else
		dejarDoblarDerecha(delta);

	if (entrada[2])			//tecla izquierda
		doblarIzquierda(delta);
	else
		dejarDoblarIzquierda(delta);

	posicion.setX(posicion.getX() + velocidad.getX());  //retocar un poco mas tal vez, multiplicar por delta la velocidad ? 
	posicion.setY(posicion.getY() + velocidad.getY());
}
void Jugador::recibirEntrada(int pos, bool estadoEntrada) {
	entrada[pos] = estadoEntrada;
}

void Jugador::acelerar(Unidad delta) {
	this->estado = EstadoAuto::DERECHO;
	if (velocidad.getY() < LIMITE_VELOCIDAD_AUTO_Y)
		this->velocidad.setY(this->velocidad.getY() + ACELERACION_AUTO_Y * delta);
	else
		velocidad.setY(LIMITE_VELOCIDAD_AUTO_Y);
}

void Jugador::doblarDerecha(Unidad delta) {
	this->estado = EstadoAuto::DOBLANDO_DER;
	if (velocidad.getX() < LIMITE_VELOCIDAD_AUTO_X)
		this->velocidad.setX(this->velocidad.getX() + ACELERACION_AUTO_X * delta);
	else
		velocidad.setX(LIMITE_VELOCIDAD_AUTO_X);
}

void Jugador::doblarIzquierda(Unidad delta) {
	this->estado = EstadoAuto::DOBLANDO_IZQ;
	if (velocidad.getX() > -LIMITE_VELOCIDAD_AUTO_X)
		this->velocidad.setX(this->velocidad.getX() - ACELERACION_AUTO_X * delta);
	else
		velocidad.setX(-LIMITE_VELOCIDAD_AUTO_X);
}

void Jugador::desacelerar(Unidad delta) {
	if (velocidad.getY() > 0)
		this->velocidad.setY(this->velocidad.getY() - ACELERACION_AUTO_Y * delta * FACTOR_DESACELERACION_X);
	else
		velocidad.setY(0);
}

void Jugador::dejarDoblarDerecha(Unidad delta) {
	if (velocidad.getX() > 0)
		this->velocidad.setY(this->velocidad.getY() - ACELERACION_AUTO_X * delta * FACTOR_DESACELERACION_Y);
	else
		velocidad.setX(0);
}

void Jugador::dejarDoblarIzquierda(Unidad delta) {
	if (velocidad.getX() < 0)
		this->velocidad.setY(this->velocidad.getY() + ACELERACION_AUTO_X * delta * FACTOR_DESACELERACION_Y);
	else
		velocidad.setX(0);
}



