#include "TcpListener.h"

#define MAX_BUFFER_SIZE (49152)

CTcpListener::CTcpListener(std::string ipAddress, int port, MessageReceiveHandler handler)
	: m_ipAddress(ipAddress), m_port(port), m_messageReceived(handler)
{
}


CTcpListener::~CTcpListener()
{
	cleanUp();
}


// send a message to the specified client
void CTcpListener::sendMessage(int clientSocket, std::string msg) {

	// echo message back to client
	send(clientSocket, msg.c_str(), msg.size() + 1, 0);
}


// initialize winsock
bool CTcpListener::init() {
	// initialize winsock
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);

	int wsOK = WSAStartup(ver, &wsData);
	if (wsOK != 0) {
		std::cerr << "Can't initialize winsock! Quitting" << std::endl;
		return false;
	}
	return true;
}


// the main processing loop
void CTcpListener::run() {

	char buffer[MAX_BUFFER_SIZE];

	// create a listening socket
	SOCKET listening = createSocket();
	if (listening == INVALID_SOCKET) {
		std::cout << "Created socket is invalid!" << std::endl;
		return;
	}

	fd_set master;
	FD_ZERO(&master);

	FD_SET(listening, &master);

	while (true) {
		fd_set copy = master;

		int socketCount = select(0, &copy, nullptr, nullptr, nullptr);

		for (int i = 0; i < socketCount; i++) {
			SOCKET sock = copy.fd_array[i];
			if (sock == listening) {
				// accept connection
				SOCKET client = accept(listening, nullptr, nullptr);

				// add the new connection to the list of connected clients
				FD_SET(client, &master);

				// send a welcome message to the connected client
				std::string welcomeMsg = "You are now connected to the Chancy Server!\n";
				send(client, welcomeMsg.c_str(), welcomeMsg.size() + 1, 0);

				// TODO: broadcast : we have a new connection
			}
			else {
				ZeroMemory(buffer, MAX_BUFFER_SIZE);

				int bytesIn = recv(sock, buffer, MAX_BUFFER_SIZE, 0);
				if (bytesIn <= 0) {
					// drop the client
					closesocket(sock);
					FD_CLR(sock, &master);
				}
				else {
					// send a message to all other clients, and NOT the listening clients
					for (int j = 0; j < master.fd_count; j++) {
						SOCKET outSock = master.fd_array[j];
						if (outSock != listening ){//&& outSock != sock) {
							send(outSock, buffer, bytesIn, 0);
						}
					}
				}
			}
		}
	}
}


// create a socket
SOCKET CTcpListener::createSocket() {
	// create a socket
	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET) {
		std::cerr << "Can't create a socket! Quitting" << std::endl;
		return -1;
	}

	// bind an ip address and port to the socket
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(m_port);
	inet_pton(AF_INET, m_ipAddress.c_str(), &hint.sin_addr);

	int bindOK = bind(listening, (sockaddr*)&hint, sizeof(hint));
	if (bindOK != SOCKET_ERROR) {
		int listenOK = listen(listening, SOMAXCONN);
		if (listenOK == SOCKET_ERROR) {
			return -1;
		}
	}
	else {
		return -1;
	}

	return listening;
}


// clean up
void CTcpListener::cleanUp() {
	WSACleanup();
}