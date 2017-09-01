#include "ServidorDeChat.h"
#include "ClienteDeChat.h"

#define ES_SERVIDOR true

int main(int argc, char* argv[]) {
	if (ES_SERVIDOR) {
		ServidorDeChat::getInstance()->iniciarServidor();
	} else {
		ClienteDeChat::getInstance()->iniciarCliente();
	}
	return 0;
}