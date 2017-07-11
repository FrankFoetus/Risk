#include <iostream>
#include <string>

#include "TcpListener.h"

void Listener_MessageReceived(CTcpListener* listener, int client, std::string msg);

void main()
{
	CTcpListener server("127.0.0.1", 54000, Listener_MessageReceived);

	if (server.init()) {
		server.run();
	}
}


void Listener_MessageReceived(CTcpListener* listener, int client, std::string msg) {
	listener->sendMessage(client, msg);
}