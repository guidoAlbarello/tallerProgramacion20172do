#include "Jugador.h"

#include <iostream>
Jugador::Jugador() {
	ObjetoDeJuego();
	camara = new Camara();
	camara->setTarget(this);
	this->texture = NULL;
	velocidad.setX(0);
	velocidad.setY(0);
}

Jugador::Jugador(SDL_Renderer* renderer) : ObjetoDeJuego(renderer) {
	//se llamo automaticamente al constructor de la superclase
	this->texture = NULL;
}
void Jugador::update(Unidad delta) {
	if (entrada[0]) 		//tecla arriba
		acelerar(delta);
	else
		desacelerar(delta);

	if (entrada[1])			//tecla derecha
		doblarDerecha(delta);
	//else
		//dejarDoblarDerecha(delta);
	else

		if (entrada[2])			//tecla izquierda
			doblarIzquierda(delta);
		else {
			dejarDoblarDerecha(delta);
			dejarDoblarIzquierda(delta);
		}

	// los 340 q aparecen son la mitaddel alcho de la pantlla menos el ancho de la imagen
	// agrande la imagen en 1/2 la veia muychica cualquir csa se puede cambiar
	//if ((posicion.getX() + velocidad.getX()) >= -340 & (posicion.getX() + velocidad.getX()) <= 340) {
	if (posicion.getX() + velocidad.getX() < LIMITE_PISTA_X_DERECHA && posicion.getX() + velocidad.getX() > LIMITE_PISTA_X_IZQUIERDA) {
		posicion.setX(posicion.getX() + velocidad.getX());  //retocar un poco mas tal vez, multiplicar por delta la velocidad ? 
	}

	posicion.setY(posicion.getY() + velocidad.getY());
}

void Jugador::recibirEntrada(int pos, bool estadoEntrada) {
	entrada[pos] = estadoEntrada;
}

void Jugador::acelerar(Unidad delta) {
	acelerando = true;
	this->estado = EstadoAuto::DERECHO;
	if (velocidad.getY() < LIMITE_VELOCIDAD_AUTO_Y)
		this->velocidad.setY(this->velocidad.getY() + ACELERACION_AUTO_Y * delta);
	else
		velocidad.setY(LIMITE_VELOCIDAD_AUTO_Y);
}

void Jugador::doblarDerecha(Unidad delta) {
	if (acelerando) {
		if (posicion.getX() + this->velocidad.getX() + ACELERACION_AUTO_X * delta < LIMITE_PISTA_X_DERECHA 
			&& posicion.getX() + this->velocidad.getX() + ACELERACION_AUTO_X * delta > LIMITE_PISTA_X_IZQUIERDA) {
			this->estado = EstadoAuto::DOBLANDO_DER;
			if (velocidad.getX() < LIMITE_VELOCIDAD_AUTO_X)
				this->velocidad.setX(this->velocidad.getX() + ACELERACION_AUTO_X * delta);
			else
				velocidad.setX(LIMITE_VELOCIDAD_AUTO_X);
		}
	}
}

void Jugador::doblarIzquierda(Unidad delta) {
	if (acelerando) {
		if (posicion.getX() - this->velocidad.getX() - ACELERACION_AUTO_X * delta < LIMITE_PISTA_X_DERECHA 
			&& posicion.getX() - this->velocidad.getX() - ACELERACION_AUTO_X * delta > LIMITE_PISTA_X_IZQUIERDA) {
			this->estado = EstadoAuto::DOBLANDO_IZQ;
			if (velocidad.getX() > -LIMITE_VELOCIDAD_AUTO_X)
				this->velocidad.setX(this->velocidad.getX() - ACELERACION_AUTO_X * delta);
			else
				velocidad.setX(-LIMITE_VELOCIDAD_AUTO_X);
		}
	}
}

void Jugador::desacelerar(Unidad delta) {
	if (velocidad.getY() > 0)
		this->velocidad.setY(this->velocidad.getY() - ACELERACION_AUTO_Y * delta * FACTOR_DESACELERACION_X);
	else {
		velocidad.setY(0);
		acelerando = false;
	}
}

void Jugador::dejarDoblarDerecha(Unidad delta) {
	//if (velocidad.getX() > 0)
	//	this->velocidad.setY(this->velocidad.getY() - ACELERACION_AUTO_X * delta * FACTOR_DESACELERACION_Y);
	//else {
		velocidad.setX(0);
		if(!entrada[2])
			this->estado = EstadoAuto::DERECHO;
	//}
}

void Jugador::dejarDoblarIzquierda(Unidad delta) {
	//if (velocidad.getX() < 0)
	//	this->velocidad.setY(this->velocidad.getY() + ACELERACION_AUTO_X * delta * FACTOR_DESACELERACION_Y);
	//else {
		velocidad.setX(0);
		if (!entrada[1])
			this->estado = EstadoAuto::DERECHO;
	//}
}



