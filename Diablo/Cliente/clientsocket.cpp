#include "clientsocket.h"
#include <iostream>

bool ClientSocket::WSinit = false;
size_t ClientSocket::ref_count = 0;

ClientSocket::ClientSocket() {
	// Increase ref count
	ref_count++;
}

ClientSocket::~ClientSocket() {
	ref_count--;
	// Clean if this is the last socket
	if(ref_count == 0) {
		WSACleanup();
	}
}

bool ClientSocket::init() {
	// Primero inicializamos winsock si no fue inicializado
	if(!WSinit) {
		WSADATA wsaData;
		int res = WSAStartup(MAKEWORD(2,2),&wsaData);
		if(res != 0) {
			std::cerr << "Inicializacion Winsock fallida: " << res << "\n";
			return false;
		}else{
			std::cout << "Winsock inicializado\n";
			WSinit = true;
		}
	}
	// Creamos el socket
	ConnectSocket = INVALID_SOCKET;
    ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(ConnectSocket == INVALID_SOCKET) {
        std::cerr << "Error at socket():" <<  WSAGetLastError() << "\n";
        return false;
    }else{
		std::cout << "Socket creado\n";
		return true;
	}
}

// Funcion de conexion (solo para clientes)
bool ClientSocket::connect(const std::string& dir, int port) {
	// Cargamos los datos de conexion
	sockaddr_in clientService;
    clientService.sin_family = AF_INET;
    clientService.sin_addr.s_addr = inet_addr(dir.c_str());
    clientService.sin_port = htons(port);
	// Creamos la conexion
	if(::connect(ConnectSocket, (SOCKADDR*)&clientService, sizeof(clientService)) == SOCKET_ERROR) {
		std::cerr << "Error conectando a " << dir << ":" << port << ": " << WSAGetLastError() << "\n";
		return false;
	}else{
		std::cout << "Conectado a " << dir << ":" << port << "\n";
		return true;
	}

}

// Funcion de send
bool ClientSocket::send(const std::string& msg) {
	int res = ::send(ConnectSocket, msg.c_str(), msg.size(), 0);
	if(res == SOCKET_ERROR) {
		std::cerr << "Error enviando mensaje: " << WSAGetLastError() << "\n";
		return false;
	}else{
		std::cout << "Se enviaron " << res << " bytes\n";
		return true;
	}
}

// Funcion de receive
bool ClientSocket::receive(std::string& buff) {
	int res = ::recv(ConnectSocket, recvbuf, DEFAULT_BUFLEN, 0);
	if(res > 0) {
		std::cout << "Received " << res << " bytes\n";
		buff.clear();
		buff.assign(recvbuf, res);
		return true;
	}else if(res == 0) {
		std::cout << "Connection closed\n";
		return false;
	}else{
		std::cout << "Receive error: " << WSAGetLastError() << "\n";
		return false;
	}
}

// Cierra el socket
void ClientSocket::close() {
	int res = closesocket(ConnectSocket);
	if(res == SOCKET_ERROR) {
		std::cerr << "Cierre de conexion fallido: " << WSAGetLastError() << "\n";
	}else{
		std::cout << "Cierre de conexion exitosa\n";
	}
}

unsigned int __stdcall ClientSocket::listenEntry(void* pthis) {
	ClientSocket* pt = (ClientSocket*)pthis;
	pt->listenDo();
	return 0;
}

void ClientSocket::listenDo() {
	std::string stuff;
	while(this->receive(stuff)) {
		std::cout << stuff;
	}
}