#include "BuzonDeMensajes.h"
#include "Logger.h"

Mensaje* BuzonDeMensajes::verMensaje(int posicionMensajeParaVer) {
	m_buzon.lock();
	Mensaje* unMensaje = buzonDeEntrada.at(posicionMensajeParaVer);
	m_buzon.unlock();
	return unMensaje;
}

void BuzonDeMensajes::eliminarMensaje(int i) {
	m_buzon.lock();
	delete this->buzonDeEntrada.at(i);
	this->buzonDeEntrada.erase(this->buzonDeEntrada.begin() +i);
	m_buzon.unlock();
}

void BuzonDeMensajes::recibirMensaje(std::string unDestinatario, std::string unEmisor, std::string unMensaje) {
	Mensaje* nuevoMensaje = new Mensaje();
	nuevoMensaje->setDestinatario(unDestinatario);
	nuevoMensaje->setEmisor(unEmisor);
	nuevoMensaje->setMensaje(unMensaje);

	m_buzon.lock();
	buzonDeEntrada.push_back(nuevoMensaje);
	m_buzon.unlock();
}

unsigned int BuzonDeMensajes::getTamanio() {
	m_buzon.lock();
	int tamanio = buzonDeEntrada.size();
	m_buzon.unlock();
	return tamanio;
}

void BuzonDeMensajes::eliminarMensajes(int posUltimoMensajeEnviado) {
	int borrados = 0;
	m_buzon.lock();
	for (int i = 0; i < posUltimoMensajeEnviado; i++) {
		borrados++;
		delete this->buzonDeEntrada[i];
	}
	buzonDeEntrada.erase(buzonDeEntrada.begin(), buzonDeEntrada.begin() + posUltimoMensajeEnviado);
	m_buzon.unlock();
}
