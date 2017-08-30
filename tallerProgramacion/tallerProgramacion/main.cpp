#include "ServidorDeChat.h"
#include "ClienteDeChat.h"

#define IP "127.0.0.1"
#define PUERTO "12345"
#define ES_SERVIDOR true
#define CANTIDAD_MAXIMA_CONEXIONES 3

int main(int argc, char* argv[]) {
	if (ES_SERVIDOR) {
		ServidorDeChat::getInstance()->iniciarServidor(PUERTO, CANTIDAD_MAXIMA_CONEXIONES);
	} else {
		ClienteDeChat::getInstance()->iniciarCliente(IP, PUERTO);
	}
	return 0;
}