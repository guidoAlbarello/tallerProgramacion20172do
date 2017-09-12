#include "SocketSincronico.h"
#include <iostream>

SocketSincronico::SocketSincronico() {
}

SocketSincronico::SocketSincronico(SOCKET unSocket) {
	this->socketDeConexion = unSocket;
}

int SocketSincronico::crearSocketCliente(string unaIp, string unPuerto) {
	WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD(2, 2);

	WSAStartup(wVersionRequested, &wsaData);
	struct addrinfo *result = NULL,
		*ptr = NULL,
		hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	int iResult = getaddrinfo(unaIp.c_str(), unPuerto.c_str(), &hints, &result);
	if (iResult != 0) {
		/*"Falla al obtener direccion y puerto"*/
		WSACleanup();
		return 1;
	}
	this->socketDeConexion = INVALID_SOCKET;

	ptr = result;

	this->socketDeConexion = socket(ptr->ai_family, ptr->ai_socktype,
		ptr->ai_protocol);

	if (this->socketDeConexion == INVALID_SOCKET) {
		/*"Socket invalido"*/
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	// Connect to server.
	iResult = connect(this->socketDeConexion, ptr->ai_addr, (int)ptr->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		/*"Fallo la conexion al servidor");*/
		iResult = closesocket(this->socketDeConexion);
		this->socketDeConexion = INVALID_SOCKET;
		WSACleanup();
		return 1;
	}

	freeaddrinfo(result);

	if (this->socketDeConexion == INVALID_SOCKET) {
		/*"No se pudo conectar al servidor"*/
		WSACleanup();
		return 1;
	}

	/*"Se pudo conectar al servidor con ip "+unaIp +" en el puerto " + unPuerto*/
	return 0;
}

int SocketSincronico::crearSocketServidor(std::string unPuerto, int conexionesMax) {
	struct addrinfo *result = NULL, *ptr = NULL, hints;
	WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD(2, 2);
	WSAStartup(wVersionRequested, &wsaData);
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the local address and port to be used by the server
	int iResult = getaddrinfo(NULL, unPuerto.c_str(), &hints, &result);
	if (iResult != 0) {
		/*"Falla al obtener direccion y puerto"*/
		WSACleanup();
		return 1;
	}
	this->socketDeConexion = INVALID_SOCKET;

	// Create a SOCKET for the server to listen for client connections

	this->socketDeConexion = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

	if (this->socketDeConexion == INVALID_SOCKET) {
		/*"Socket invalido"*/
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	iResult = bind(this->socketDeConexion, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		/*"Fallo el bind del socket"*/
		freeaddrinfo(result);
		closesocket(this->socketDeConexion);
		WSACleanup();
		return 1;
	}

	freeaddrinfo(result);

	int socketResponse = listen(this->socketDeConexion, conexionesMax);
	if (socketResponse == SOCKET_ERROR) {
		closesocket(this->socketDeConexion);
		WSACleanup();
		return 1;
	}
	/*"Se creo el socket del servidor escuchando en el puerto " + unPuerto*/
	return iResult;
}

bool SocketSincronico::enviarDatos(const char* datosAEnviar, int tamanio) {
	int tamanioDeDatosEnviados;
	// Send an initial buffer
	if (tamanio <= 0) {
		return true;
	}
	tamanioDeDatosEnviados = send(this->socketDeConexion, (char*)&tamanio, sizeof(tamanio), 0);
	if (tamanioDeDatosEnviados == sizeof(tamanio)) {
		int cantidadDatosEnviados = 0;
		while (cantidadDatosEnviados < tamanio) {
			int datosEnviados = send(this->socketDeConexion, (char*)datosAEnviar + cantidadDatosEnviados, tamanio - cantidadDatosEnviados, 0);
			if (datosEnviados == SOCKET_ERROR) {
				closesocket(this->socketDeConexion);
				return false;
			}
			if (datosEnviados > 0) {
				cantidadDatosEnviados += datosEnviados;
			}
			else {
				/*Falla al enviar datos */
				closesocket(this->socketDeConexion);
				return false;
			}
		}
	}
	else if (tamanioDeDatosEnviados == SOCKET_ERROR) {
		/*Falla al enviar datos */
		closesocket(this->socketDeConexion);
		return false;
	}

	/*"Se enviaron los datos correctamente"*/
	return true;
}

char* SocketSincronico::recibirDatos() {
	int tamanioDeDatosRecibidos;
	int tamanio;
	void* datosARecibir = NULL;
	// Send an initial buffer
	tamanioDeDatosRecibidos = recv(this->socketDeConexion, (char*)&tamanio, sizeof(tamanio), 0);
	if (tamanioDeDatosRecibidos > 0) {
		if (tamanioDeDatosRecibidos == sizeof(tamanio)) {
			int cantidadDatosRecibidos = 0;
			datosARecibir = malloc(tamanio);
			while (cantidadDatosRecibidos < tamanio) {
				int datosRecibidos = recv(this->socketDeConexion, (char*)datosARecibir + cantidadDatosRecibidos, tamanio - cantidadDatosRecibidos, 0);
				if (datosRecibidos > 0) {
					cantidadDatosRecibidos += datosRecibidos;
				}
				else {
					/*"Falla al recibir datos"*/
					closesocket(this->socketDeConexion);
					return (char*)datosARecibir;
				}
			}
		}
		else if (tamanioDeDatosRecibidos <= 0) {
			closesocket(this->socketDeConexion);

			/*"Falla al recibir datos"*/
			//return (char*)datosARecibir;
		}

		/*"Se recibieron los datos correctamente"*/
		string mensaje((char *)datosARecibir);
		//Logger::getInstance()->log(Debug, "datos recibidos: " + mensaje);
		//cout << "|----------------------------|" << mensaje;
	}
	return (char*)datosARecibir;
}

int SocketSincronico::cerrarSocket() {
	if (this->socketDeConexion != INVALID_SOCKET) {
		int iResult = shutdown(this->socketDeConexion, SD_SEND);
		if (iResult == SOCKET_ERROR) {
			closesocket(this->socketDeConexion);
			WSACleanup();
			/*"Falla al cerrar el socket"*/
			return 1;
		}

		closesocket(this->socketDeConexion);
		this->socketDeConexion = INVALID_SOCKET;
		WSACleanup();
		/*"Se cerro el socket correctamente"*/
		return 0;
	}
	else {
		/*"Falla al cerrar el socket"*/
		WSACleanup();
		return 1;
	}
}

int SocketSincronico::hayClienteIntentandoConectarse() {
	SOCKET socketDelCliente = INVALID_SOCKET;

	// Accept a client socket
	socketDelCliente = accept(this->socketDeConexion, NULL, NULL);
	return socketDelCliente;
}
