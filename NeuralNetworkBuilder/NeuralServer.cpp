/****************************************************************************************************************/
/*                                                                                                              */
/*   NNE - Neural Networks Engine                                                                               */
/*   www.sapirshemer.com                                                                                        */
/*                                                                                                              */
/*   Neural Server CLASS                                                                                        */
/*                                                                                                              */
/*   Sapir Shemer                                                                                               */
/*   DevShift - Shifting development forward                                                                    */
/*   me@sapirshemer.com                                                                                         */
/*                                                                                                              */
/****************************************************************************************************************/

#include "NeuralServer.h"



NeuralServer::NeuralServer(const char * port)
{
	this->port = port;
	addrinfo * result = NULL;

	// Initialize Winsock
	int error = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (error != 0) {
		std::cerr << "[-] NNS: WSAStartup failed with error: " << error << std::endl;
		return;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the server address and port
	error = getaddrinfo(NULL, this->port, &hints, &result);
	if (error != 0) {
		std::cerr << "[-] NNS: getaddrinfo failed with error: " << error << std::endl;
		WSACleanup();
		return;
	}

	// Create a SOCKET for connecting to server
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ListenSocket == INVALID_SOCKET) {
		std::cerr << "[-] NNS: socket failed with error: %ld\n" << WSAGetLastError() << std::endl;
		freeaddrinfo(result);
		WSACleanup();
		return;
	}

	// Setup the TCP listening socket
	error = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (error == SOCKET_ERROR) {
		std::cerr << "[-] NNS: bind failed with error: " << WSAGetLastError() << std::endl;
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		return;
	}

	freeaddrinfo(result);

	error = listen(ListenSocket, SOMAXCONN);
	if (error == SOCKET_ERROR) {
		std::cerr << "listen failed with error: " << WSAGetLastError() << std::endl;
		closesocket(ListenSocket);
		WSACleanup();
		return;
	}
}


NeuralServer::~NeuralServer()
{
}

void NeuralServer::RunServer()
{
	while (true)
	{
		struct sockaddr_in * client_addr;
		socklen_t  client_addr_size = sizeof(client_addr);
		int error = 0;

		// Accept a client socket
		ClientSocket = accept(ListenSocket, (struct sockaddr *)&client_addr, &client_addr_size);
		if (ClientSocket == INVALID_SOCKET) {
			printf("accept failed with error: %d\n", WSAGetLastError());
			closesocket(ListenSocket);
			WSACleanup();
			return;
		}

		// Get address of client
		char addr[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &client_addr->sin_addr, addr, INET_ADDRSTRLEN);
		std::string ip(addr);

		if (std::find(this->devices_ids.begin(), this->devices_ids.end(), ip) != this->devices_ids.end())
		{
			// Add ip to collection
			this->devices_ids.push_back(ip);

			// Recieve initial data
			error = recv(ClientSocket, recv_data, 1024, 0);

			// Determine type of connection and move proccessing to another thread
			if (error > 0) {
				// Get data as string
				std::string data(recv_data);

				// Add a new machine
				if (data.find(MACHINE_CODE) != std::string::npos)
				{
					// Manage machine on a new thread
					this->managed_connections.push_back(std::thread(ManageMachine, ClientSocket));
					break;
				}

				if (data.find(CLIENT_CODE) != std::string::npos)
				{
					// Manage client on a new thread
					this->managed_connections.push_back(std::thread(ManageClient, ClientSocket));
					break;
				}
			}
			else {
				printf("recv failed with error: %d\n", WSAGetLastError());
				closesocket(ClientSocket);
				WSACleanup();
				return;
			}
		}
	}
}

void NeuralServer::SendData(const char * data)
{
	// Send an initial buffer
	int error = send(this->ClientSocket, data, (int)strlen(data), 0);
	if (error == SOCKET_ERROR) {
		std::cerr << "[-] NNC: Send failed with error: " << WSAGetLastError() << std::endl;
		closesocket(this->ClientSocket);
		WSACleanup();
		return;
	}
}

void NeuralServer::ManageMachine(SOCKET socket)
{
}

void NeuralServer::ManageClient(SOCKET socket)
{
}
