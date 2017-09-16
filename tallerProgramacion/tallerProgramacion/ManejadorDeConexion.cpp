#include "ManejadorDeConexion.h"

ManejadorDeConexion::ManejadorDeConexion() {
	this->socket = new SocketSincronico();
	bufferDatosAEnviar = NULL;
	bufferDatosRecibidos = NULL;
	conexionActiva = true;
}

ManejadorDeConexion::ManejadorDeConexion(SOCKET unSocket) {
	this->socket = new SocketSincronico(unSocket);
	bufferDatosAEnviar = NULL;
	bufferDatosRecibidos = NULL;
	conexionActiva = true;
}

ManejadorDeConexion::ManejadorDeConexion(SocketSincronico* unSocket) {
	this->socket = unSocket;
	bufferDatosAEnviar = NULL;
	bufferDatosRecibidos = NULL;
	conexionActiva = true;
}

void  ManejadorDeConexion::notificarDatosAEnviar() {
	condicion_enviar.notify_all();
}

void  ManejadorDeConexion::notificarDatoARecibir() {
	condicion_recibir.notify_all();
}

void ManejadorDeConexion::enviarDatos() {
	/*
	//Por ahora no se usa.. quizas en la 2da entrega
	while (conexionActiva) {
		condicion_enviar.wait(m_bufferDatosAEnviar);
		//espera hasta que alguien haga condicion_enviar.notifyAll();
		if (bufferDatosAEnviar != NULL) {
			if (this->socket->enviarDatos(bufferDatosAEnviar, tamanioDatosAEnviar)) {
				bufferDatosAEnviar = NULL;
			}
		}
	}
	*/
}

void ManejadorDeConexion::recibirDatos() {
	
	while (conexionActiva) {
		//
		char *datosRecibidos = this->socket->recibirDatos();
		if (datosRecibidos != NULL) {
			//PAra recibir datos no haria falta, porque el recv es bloqueante y este proceso no se envia datos a si mismo 
			//condicion_recibir.wait(m_bufferDatosRecibidos);
			m_bufferDatosRecibidos.lock();
			bufferDatosRecibidos = datosRecibidos;     //LIBERAR MEMORIA
			m_bufferDatosRecibidos.unlock();
		}
	}
}

void ManejadorDeConexion::cerrarConexion() {
	this->conexionActiva = false;

	if (t_EnviarDatos.joinable()) {
		t_EnviarDatos.join();
	}

	if (t_RecibirDatos.joinable()) {
		t_RecibirDatos.join();
	}

	socket->cerrarSocket();
	delete socket;
}