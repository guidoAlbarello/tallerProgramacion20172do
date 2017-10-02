#pragma once
#ifndef MENSAJE_H
#define MENSAJE_H
#include <string>
class Mensaje {
public:
	void setDestinatario(std::string unDestinatario) { this->destinatario = unDestinatario; }
	void setEmisor(std::string unEmisor) { this->emisor = unEmisor; }
	void setMensaje(std::string unMensaje) { this->mensaje = unMensaje; }
	std::string getDestinatario() { return this->destinatario; }
	std::string getEmisor() { return this->emisor; }
	std::string getMensaje() { return this->mensaje; }
private:
	std::string destinatario;
	std::string emisor;
	std::string mensaje;
};

#endif