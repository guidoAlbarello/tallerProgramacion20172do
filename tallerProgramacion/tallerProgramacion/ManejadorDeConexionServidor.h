#include "ManejadorDeConexion.h"

class ManejadorDeConexionServidor : public ManejadorDeConexion {
public:
	void iniciarConexion(std::string puertoEscucha, int cantidadConexionesMaxima);
	SOCKET hayClienteIntentandoConectarse();
protected:
	std::thread t_escucharClientes;

};