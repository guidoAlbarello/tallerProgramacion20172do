#include "Servidor.h"
#include "Cliente.h"

using namespace std;

#define ES_SERVIDOR true

int main(int argc, char* argv[]) {
	cout << "-------------------------------" << std::endl;
	cout << "Menu" << std::endl;
	cout << "-------------------------------" << std::endl;
	cout << "1.Ejecutar Servidor" << std::endl;
	cout << "2. Ejecutar Cliente" << std::endl;


	char entradaUsuario;
	std::cin >> entradaUsuario;
	switch (entradaUsuario) {
	case '1':
		Servidor::getInstance()->iniciarServidor();
		break;
	case '2':
		Cliente::getInstance()->iniciarCliente();
		break;
	default:
		break;
	}

	return 0;
}
