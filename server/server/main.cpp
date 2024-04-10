#include <iostream>
#include <WS2tcpip.h>

#pragma comment (lib, "ws2_32.lib")

void main()
{

	// Initializing winsock
	std::cout << "[i] Initializing winsock" << std::endl;

	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);

	int wsOK = WSAStartup(ver, &wsData);
	
	if (wsOK != 0)
	{
		std::cerr << "Can't initialize winsock, exiting" << std::endl;
		return;
	}

	// Create socket
	std::cout << "[i] Creating socket" << std::endl;

	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);

	if (listening == INVALID_SOCKET)
	{
		std::cerr << "Can't create socket, exiting" << std::endl;
		return;
	}

	// Bind local IP and port to socket
	std::cout << "[i] Binding to socket" << std::endl;

	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(55000);
	hint.sin_addr.S_un.S_addr = INADDR_ANY;

	bind(listening, (sockaddr*)&hint, sizeof(hint));

	// Tell winsock the socket is listening
	listen(listening, SOMAXCONN);

	// Wait for connection
	sockaddr_in client;
	int clientSize = sizeof(client);

	SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clientSize);

	if (clientSocket == INVALID_SOCKET)
	{
		std::cerr << "Invalid client socket" << std::endl;
	}
	
	char host[NI_MAXHOST];		// client's name
	char service[NI_MAXHOST];	// Service the client is connected on

	ZeroMemory(host, NI_MAXHOST);
	ZeroMemory(service, NI_MAXSERV);

	if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
	{
		std::cout << host << " has connected on port " << service << std::endl;
	}
	else {
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		std::cout << host << " has connected on port " << ntohs(client.sin_port) << std::endl;
		
	}

	// Close listening socket

	closesocket(listening);

	// While loop: accept and echo message to client
	char buf[4096];

	while (true)
	{
		ZeroMemory(buf, 4096);

		// Wait for client to send data

		int bytesReceived = recv(clientSocket, buf, 4096, 0);
		if (bytesReceived == SOCKET_ERROR) 
		{
			std::cerr << "Error in recv(), exiting" << std::endl;
			return;
		}
		if (bytesReceived == 0)
		{
			std::cout << "Client Disconnected" << std::endl;
			break;
		}

		// Echo message back to client

		send(clientSocket, buf, bytesReceived + 1, 0);

	}

	// Close socket

	closesocket(clientSocket);

	// Cleanup winsock

	WSACleanup();
}