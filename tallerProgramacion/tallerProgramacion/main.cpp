#include "ServidorDeChat.h"
#include "ClienteDeChat.h"

#define IP "127.0.0.1"
#define PUERTO "12345"
#define ES_SERVIDOR true
//#define CANTIDAD_MAXIMA_CONEXIONES 3

int main(int argc, char* argv[]) {
	if (ES_SERVIDOR) {
		ServidorDeChat::getInstance()->iniciarServidor();
	} else {
		ClienteDeChat::getInstance()->iniciarCliente();
	}
	return 0;
}