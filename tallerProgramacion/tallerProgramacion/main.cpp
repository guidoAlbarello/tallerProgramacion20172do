#include "Servidor.h"
#include "Cliente.h"

#define IP "127.0.0.1"
#define PUERTO "12345"
#define ES_SERVIDOR true

int main(int argc, char* argv[]) {
	if (ES_SERVIDOR) {
		Servidor* nuevoServidor = new Servidor();
		nuevoServidor->iniciarServidor();

	} else {
		Cliente* nuevoCliente = new Cliente(); 
		nuevoCliente->iniciarCliente();
	}
	return 0;
}