#pragma once
#pragma comment (lib, "ws2_32.lib")

#include <string>
#include <iostream>
#include <WS2tcpip.h>

// forward declare class
class CTcpListener;

// callback to data received
typedef void(*MessageReceiveHandler)(CTcpListener* listener, int socketId, std::string msg);


class CTcpListener
{

public:
	CTcpListener(std::string ipAddress, int port, MessageReceiveHandler handler);
	~CTcpListener();

	// send a message to the specified client
	void sendMessage(int clientSocket, std::string msg);

	// initialize winsock
	bool init();

	// create socket
	SOCKET createSocket();

	// the main processing loop
	void run();

	void cleanUp();

private:

	std::string				m_ipAddress;
	int						m_port;
	MessageReceiveHandler	m_messageReceived;
};

