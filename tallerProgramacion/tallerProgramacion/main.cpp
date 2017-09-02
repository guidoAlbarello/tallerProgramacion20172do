#include "Servidor.h"
#include "Cliente.h"

#define ES_SERVIDOR true

int main(int argc, char* argv[]) {
	if (ES_SERVIDOR) {
		Servidor::getInstance()->iniciarServidor();
	} else {
		Cliente::getInstance()->iniciarCliente();
	}
	return 0;
}