#include "ManejadorDeConexion.h"

using namespace std;

class ManejadorDeConexionCliente : public ManejadorDeConexion {
public:
	void iniciarConexion(string ipServidor, string puertoServidor);
	//bool ejecutarComando(Comando comando, std::stri, std::string puertoServidor);
	bool login(string user, string pass);
	bool enviarMensajeGlobal(string unMensaje);
	bool enviarMensajePrivado(string unDestinatario, string unMensaje);
	bool realizarPeticionUsuarios();
	bool devolverMensajesPrivados();
	bool enviarSolicitudPing();
	bool enviarRespuestaPingAServidor();
};