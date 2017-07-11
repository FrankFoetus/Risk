#include "Multiplayer.h"



Multiplayer::Multiplayer(std::string ipAddress, int port)
	: m_ipAddress(ipAddress), m_port(port)
{
	initClient();
}


Multiplayer::~Multiplayer()
{
	clearUp();
}


void Multiplayer::initClient() {
	// Winsock Startup
	WSAData wsaData;
	WORD DllVersion = MAKEWORD(2, 2);
	int wsResult = WSAStartup(DllVersion, &wsaData);
	if (wsResult != 0) {
		std::cerr << "Can't start Winsock, Err #" << wsResult << std::endl;
		return;
	}

	connectToServer();
}

void Multiplayer::connectToServer() {
	// create socket
	m_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (m_socket == INVALID_SOCKET) {
		std::cerr << "Can't create socket, Err #" << WSAGetLastError() << std::endl;
		return;
	}

	// fill in a hint structure
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(m_port);
	inet_pton(AF_INET, m_ipAddress.c_str(), &hint.sin_addr);

	// connect to server
	int connResult = connect(m_socket, (sockaddr*)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR) {
		std::cerr << "Can't connect to server, Err #" << WSAGetLastError() << std::endl;
		closesocket(m_socket);
		WSACleanup();
		return;
	}
}


void Multiplayer::sendData(std::string msg) {
	// do while loop to send and receive data
	char buffer[256];

	// make sure the user typed in something
	if (msg.size() > 0) {
		// send the text
		int sendResult = send(m_socket, msg.c_str(), msg.size() + 1, 0);
		if (sendResult != SOCKET_ERROR) {
			// wait for response
			ZeroMemory(buffer, 256);
			int bytesReceived = recv(m_socket, buffer, 256, 0);
			if (bytesReceived > 0) {
				// echo response to console
				std::cout << "SERVER> " << std::string(buffer, 0, bytesReceived) << std::endl;
			}
		}
	}
}


void Multiplayer::clearUp(){
	// gracefully close down everything
	closesocket(m_socket);
	WSACleanup();
}