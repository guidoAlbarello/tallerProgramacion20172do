#include "ManejadorDeConexion.h"

class ManejadorDeConexionCliente : public ManejadorDeConexion {
public:
	void iniciarConexion(std::string ipServidor, std::string puertoServidor);
protected:
	char* procesarDatosAEnviar();
	char* procesarDatosRecibidos(char* datosRecibidos);
};