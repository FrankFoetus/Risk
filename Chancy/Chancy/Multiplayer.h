
#pragma comment(lib, "ws2_32.lib")
#pragma once

#include <ws2tcpip.h>
#include <iostream>
#include <string>

class Multiplayer
{

public:
	Multiplayer(std::string ipAddress, int port);
	~Multiplayer();

	void sendData(std::string msg);

	// setters
	void setIP(std::string ipAddress) { m_ipAddress = ipAddress; }
	void setPort(int port) { m_port = port; }

	// getters
	std::string getIP() { return m_ipAddress; }
	int getPort() { return m_port; }

private:
	void initClient();
	void connectToServer();
	void clearUp();

	std::string				m_ipAddress;
	int						m_port;
	SOCKET					m_socket;
};

