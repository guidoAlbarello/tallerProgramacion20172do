#include "ServidorDeCorreo.h"
#include "ClienteDeCorreo.h"

#define IP "127.0.0.1"
#define PUERTO "12345"
#define ES_SERVIDOR true
#define CANTIDAD_MAXIMA_CONEXIONES 3

int main(int argc, char* argv[]) {
	if (ES_SERVIDOR) {
		Servidor* nuevoServidor = new ServidorDeCorreo();
		nuevoServidor->iniciarServidor(PUERTO, CANTIDAD_MAXIMA_CONEXIONES);
	} else {
		Cliente* nuevoCliente = new ClienteDeCorreo(); 
		nuevoCliente->iniciarCliente(IP, PUERTO);
	}
	return 0;
}