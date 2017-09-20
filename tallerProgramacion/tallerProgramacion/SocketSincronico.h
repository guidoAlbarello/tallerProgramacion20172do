#pragma once
#ifndef  SOCKETSINCRONICO_H
#define SOCKETSINCRONICO_H

#pragma comment(lib,"ws2_32")

#include <winsock2.h>
#include <WS2tcpip.h>
#include <string>


using namespace std;

class SocketSincronico {
public:
	SocketSincronico();
	SocketSincronico(SOCKET unSocket);
	int crearSocketCliente(std::string unaIp, std::string unPuerto);
	int crearSocketServidor(std::string unPuerto, int cantidadMaximaDeConexiones);
	bool enviarDatos(const char* datosAEnviar, int tamanio);
	char* recibirDatos();
	int cerrarSocket();
	int hayClienteIntentandoConectarse();
	SOCKET getSocket() { return this->socketDeConexion; };
	void borrarEntornoWSA();
private:
	SOCKET socketDeConexion;
};

#endif