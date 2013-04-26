#pragma once
#pragma comment(lib, "Ws2_32.lib")

#include <string>

#include <winsock2.h>
#include <ws2tcpip.h>

// Tama�o del buffer de envio/recepcion
const size_t DEFAULT_BUFLEN = 1024;

class ClientSocket {
	private:
	// Init de winsock
	static bool WSinit;
	// Contador para hacer el manejor de recursos
	static size_t ref_count;
	// Variables para manejo del socket
	SOCKET ConnectSocket;
	char recvbuf[DEFAULT_BUFLEN];

	public:
	// Constructor
	ClientSocket();
	// Destructor
	~ClientSocket();
	// Funcion de inicializacion
	bool init();
	// Funcion de conexion (clientes)
	bool connect(const std::string& dir, int port);
	// Funcion bloqueante de recepcion
	bool receive(std::string& buff);
	// Funcion de send
	bool send(const std::string& msg);
	// Cierra el socket
	void close();
};