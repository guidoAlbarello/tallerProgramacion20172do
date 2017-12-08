#include "Jugador.h"

#include <iostream>
Jugador::Jugador(string nombre) {
	ObjetoDeJuego();
	camara = new Camara();
	camara->setTarget(this);
	this->texture = NULL;
	velocidad.setX(0);
	velocidad.setY(0);
	velocidadMaxima = LIMITE_VELOCIDAD_AUTO_Y_PISTA;
	this->chocado = false;
	this->puntos = 0;
	this->nombre = nombre;
	this->tiempo = 0;

	for (int i = 0; i < Constantes::CANT_TECLAS; i++) {
		entrada[i] = 0;
	}
}

Jugador::Jugador(SDL_Renderer* renderer) : ObjetoDeJuego(renderer) {
	//se llamo automaticamente al constructor de la superclase
	this->texture = NULL;
}
void Jugador::update(Unidad delta) {
	//auto time = sc::system_clock::now(); // get the current time
	//auto since_epoch = time.time_since_epoch(); // get the duration since epoch
	//auto millis = sc::duration_cast<sc::milliseconds>(since_epoch);
	//long now = millis.count();

	//Uint32 inicio = 0;
	//Uint32 fin = 0;
	Uint32 intervaloNitro = 0;

	Uint32 inicio = SDL_GetTicks();

	//fin = SDL_GetTicks();
	//intervaloNitro = fin - inicio;

	//if (intervaloNitro > Constantes::PING_DELAY);

	//if (!chocado) {

	

		if (entrada[3]) {
			//if (!usarNitro) {

			
			if (!prohibirUsoNitro) {
				if (!usarNitro) {
					tiempoNitro = 0;
					inicioIntervalo = chrono::high_resolution_clock::now();
				}
				usarNitro = true;
			}


			//if (usarNitro && tiempoNitro >= 10 * 1000) {
			//	usarNitro = false;
			//	prohibirUsoNitro = true;
			//}

			//if (now - tiempoUltimoNitro > 2000) {
				
			//}
			//}
		}
		else {
			if (usarNitro) {
				// prohibir uso nit
				prohibirUsoNitro = true;
			}
			usarNitro = false;
		}
		if (usarNitro) {
			auto finIntervalo = chrono::high_resolution_clock::now();
			auto dur = finIntervalo - inicioIntervalo;
			auto ms = std::chrono::duration_cast<std::chrono::nanoseconds>(dur).count();
			inicioIntervalo = chrono::high_resolution_clock::now();
			tiempoNitro += ms / 1000000.0;
		}

		if (usarNitro && tiempoNitro >= 10 * 1000) {
			usarNitro = false;
			prohibirUsoNitro = true;
		}

		/*if (entrada[1])
		movimientoDeshabilitado = true;*/
		if (!movimientoDeshabilitado) {
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

				int limitePastoXDerecha = ANCHO_TRAMO / 2 + 90;
				int limitePastoXIzquierda = ANCHO_TRAMO / 2 * (-1) - 90;

				if ((posicion.getX() > limitePastoXDerecha) ||
					(posicion.getX() < limitePastoXIzquierda)) {
					velocidadMaxima = LIMITE_VELOCIDAD_AUTO_Y_PASTO;
				}
				else {
					velocidadMaxima = LIMITE_VELOCIDAD_AUTO_Y_PISTA;
				}

				posicion.setY(posicion.getY() + velocidad.getY());
		}
		else {
			velocidad.setY(0);
			velocidad.setX(0);
		}

		intervaloNitro += inicio - SDL_GetTicks();
		if (intervaloNitro > 2000) {
			prohibirUsoNitro = false;
			//usarNitro = true;
			//tiempoNitro = 0;
			//inicioIntervalo = chrono::high_resolution_clock::now();

			intervaloNitro = 0;
		}
	//}
}

void Jugador::recibirEntrada(int pos, bool estadoEntrada) {
	entrada[pos] = estadoEntrada;
}

void Jugador::acelerar(Unidad delta) {
	acelerando = true;
	this->estado = EstadoAuto::DERECHO;
	if (!usarNitro) {
		if (this->velocidad.getY() + ACELERACION_AUTO_Y * delta < velocidadMaxima)
			this->velocidad.setY(this->velocidad.getY() + ACELERACION_AUTO_Y * delta);
		else
			velocidad.setY(velocidadMaxima);
	}
	else {
		if (this->velocidad.getY() + ACELERACION_NITRO * delta < LIMITE_VELOCIDAD_AUTO_NITRO)
			this->velocidad.setY(this->velocidad.getY() + ACELERACION_NITRO * delta);
		else
			velocidad.setY(LIMITE_VELOCIDAD_AUTO_NITRO);
	}
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
	if (velocidad.getY() > 0) {
		this->velocidad.setY(this->velocidad.getY() - FACTOR_DESACELERACION_Y * delta);
		if (this->velocidad.getY() < 0) {
			this->velocidad.setY(0);
		}
	}
	else if (velocidad.getY() < 0) {
		this->velocidad.setY(this->velocidad.getY() + FACTOR_DESACELERACION_Y * delta);
		if (this->velocidad.getY() > 0) {
			this->velocidad.setY(0);
		}
	}
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
	if (!entrada[2])
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

bool Jugador::estaChocado() {
	return this->chocado;
}

void Jugador::chocar(double posicionY, int velocidad) {
	//auto time = sc::system_clock::now(); // get the current time
	//auto since_epoch = time.time_since_epoch(); // get the duration since epoch
	//auto millis = sc::duration_cast<sc::milliseconds>(since_epoch);
	//long now = millis.count();
	dañarAuto();


	//if (now - tiempoDeChocado > 1000) {
	//	tiempoDeChocado = now;
	//	dañarAuto();
	//}
	this->velocidad.setX(0);
	if (velocidad >= 40) {
		this->velocidad.setY(velocidad - 40);
	}
	else {
		this->velocidad.setY(0);
	}
	this->velocidad.setY(-20);
	//this->setPosicionY(posicionY - 190);
	//this->setPosicionX(posicion.getX() - 30);

	// Logica sonido choque con timer, TODO!
	this->sonidoChoque = true;

}

long Jugador::getPuntos() {
	return this->puntos;
}

void Jugador::addPuntos(long agregar) {
	this->puntos += agregar;
}


void Jugador::dañarAuto() {
	if (vida > 0)
		vida--;
	else
		vida = 0;
}
long Jugador::getTiempo() {
	return this->tiempo;
}
void Jugador::setTiempo(long t) {
	this->tiempo = t;
}

