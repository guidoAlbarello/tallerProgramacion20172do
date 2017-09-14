#pragma once
#ifndef BuzonDeMensajes_H
#define BuzonDeMensajes_H

#include "Mensaje.h"
#include <mutex>
#include <vector>
class BuzonDeMensajes {
public:
	Mensaje* verMensaje(int posicionMensajeParaVer);
	void recibirMensaje(std::string unDestinatario, std::string unEmisor, std::string unMensaje);
	unsigned int getTamanio();
	void eliminarMensajes(int posUltimoMensajeEnviado);
private:
	std::vector<Mensaje*> buzonDeEntrada;
	std::mutex m_buzon;
};

#endif