#include "ManejadorDeConexion.h"

class ManejadorDeConexionServidor : public ManejadorDeConexion {
public:
	void iniciarConexion(std::string puertoEscucha, int cantidadConexionesMaxima);
protected:
	char* procesarDatosAEnviar();
	char* procesarDatosRecibidos(char* datosRecibidos);
};