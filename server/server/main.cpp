#include <iostream>
#include <WS2tcpip.h>
#include <string>
#include <sstream>

#pragma comment (lib, "ws2_32.lib")

void main()
{

	// Initializing winsock
	std::cout << "[i] Initializing hChat Server" << std::endl;
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

	// Create file descriptor and zero it
	fd_set master;
	FD_ZERO(&master);

	// Add listening socket
	FD_SET(listening, &master);

	while (true)
	{
		fd_set copy = master;

		int socketCount = select(0, &copy, nullptr, nullptr, nullptr);

		for (int i = 0; i < socketCount; i++)
		{
			SOCKET sock = copy.fd_array[i];

			if (sock == listening)
			{
				// Accept a new conection
				SOCKET client = accept(listening, nullptr, nullptr);

				// Add the new connection to the list of clients
				FD_SET(client, &master);

				// Log incoming connection

				std::cout << "[!] New socket connected: SOCKET #" << sock << std::endl;

				// Send a welcome message
				std::string MOTD = "Welcome to hChat Server!\r\n";
				
				send(client, MOTD.c_str(), MOTD.size() + 1, 0);
				
			}
			else 
			{
				char buf[4096];
				ZeroMemory(buf, 4096);
				
				// Receive message

				int bytesIn = recv(sock, buf, 4096, 0);

				if (bytesIn <= 0)
				{
					// Drop client
					closesocket(sock);
					FD_CLR(sock, &master);
				}
				else 
				{
					// Send message to the other clients, not the listening socket
					for (int i = 0; i < master.fd_count; i++)
					{
						SOCKET outSock = master.fd_array[i];
						if (outSock != listening && outSock != sock)
						{
							std::ostringstream ss;
							ss << "SOCKET #" << sock << ": " << buf << "\r\n";
							std::string strOut = ss.str();
							
							std::cout << "[M] " << strOut << std::endl;

							send(outSock, strOut.c_str(), strOut.size() + 1, 0);
						}
					}

				}
				
			}

		}
	}

	// Cleanup winsock

	WSACleanup();
}