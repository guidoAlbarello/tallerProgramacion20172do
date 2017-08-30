#include "BuzonDeMensajes.h"

Mensaje* BuzonDeMensajes::verMensaje(int posicionMensajeParaVer) {
	m_buzon.lock();
	Mensaje* unMensaje = buzonDeEntrada.at(posicionMensajeParaVer);
	m_buzon.unlock();
	return unMensaje;
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
