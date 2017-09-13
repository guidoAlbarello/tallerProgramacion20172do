#include "Servidor.h"
#include "Cliente.h"

using namespace std;

#define ES_SERVIDOR true

int main(int argc, char* argv[]) {
	std::string input;
	Logger::getInstance()->setMode(Debug);
	Logger::getInstance()->log(Debug, "Iniciando aplicacion");
	while (input.length() != 1) {
		cout << "|----------------------------|" << std::endl;
		cout << "|            Menu            |" << std::endl;
		cout << "|----------------------------|" << std::endl;
		cout << "1.Ejecutar Servidor" << std::endl;
		cout << "2.Ejecutar Cliente" << std::endl;
		cout << "3.Salir" << std::endl;

		std::getline(std::cin, input);
		if (input.length() != 1) {
			std::cout << "Debe ingresar una de las opciones" << std::endl;
		}
		else {
			char opcionElegida = input[0];
			switch (opcionElegida) {
			case '1':
				Servidor::getInstance()->iniciarServidor();
				break;
			case '2':
				Cliente::getInstance()->iniciarCliente();
				break;
			default:
				break;
			}
			break;
		}
	}
			
	return 0;
}
