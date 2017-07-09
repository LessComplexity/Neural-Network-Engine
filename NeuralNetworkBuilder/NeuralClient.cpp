/****************************************************************************************************************/
/*                                                                                                              */
/*   NNE - Neural Networks Engine                                                                               */
/*   www.sapirshemer.com                                                                                        */
/*                                                                                                              */
/*   NeuralClient CLASS                                                                                               */
/*                                                                                                              */
/*   Sapir Shemer                                                                                               */
/*   DevShift - Shifting development forward                                                                    */
/*   me@sapirshemer.com                                                                                         */
/*                                                                                                              */
/****************************************************************************************************************/

#include "NeuralClient.h"



NeuralClient::NeuralClient()
{
	// Startup winsock
	int error = WSAStartup(MAKEWORD(2, 2), &this->wsadata);
	if (error)
	{
		std::cerr << "[-] NNC: Couldn't start socket";
		return;
	}

	ZeroMemory(&this->hints, sizeof(this->hints));
	this->hints.ai_family = AF_UNSPEC;
	this->hints.ai_socktype = SOCK_STREAM;
	this->hints.ai_protocol = IPPROTO_TCP;
}

NeuralClient::NeuralClient(const char * server_ip, const char * server_port)
{
	this->server_ip = server_ip;
	this->server_port = server_port;
	// Startup winsock
	int error = WSAStartup(MAKEWORD(2, 2), &this->wsadata);
	if (error)
	{
		std::cerr << "[-] NNC: Couldn't start socket";
		return;
	}

	ZeroMemory(&this->hints, sizeof(this->hints));
	this->hints.ai_family = AF_UNSPEC;
	this->hints.ai_socktype = SOCK_STREAM;
	this->hints.ai_protocol = IPPROTO_TCP;
}


NeuralClient::~NeuralClient()
{
}

void NeuralClient::Connect(const char * server_ip, const char * server_port)
{
	this->server_ip = server_ip;
	this->server_port = server_port;
	Connect();
}

void NeuralClient::Connect()
{
	if (this->server_ip != NULL)
	{
		// Variable for storing the result
		addrinfo * result = NULL;
		// Resolve the server address and port
		int error = getaddrinfo(server_ip, server_port, &this->hints, &result);
		if (error != 0) {
			std::cerr << "[-] NNC: Couldn't resolve address: " << error;
			WSACleanup();
			return;
		}

		// Attempt to connect to an address until one succeeds
		for (addrinfo * ptr = result; ptr != NULL; ptr->ai_next)
		{
			// Create a SOCKET for connecting to server
			this->sock = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
			if (this->sock == INVALID_SOCKET) {
				std::cerr << "[-] NNC: Couldn't create socket: " << WSAGetLastError();
				WSACleanup();
				return;
			}

			// Connect to server.
			int error = connect(this->sock, ptr->ai_addr, (int)ptr->ai_addrlen);
			if (error == SOCKET_ERROR) {
				closesocket(this->sock);
				this->sock = INVALID_SOCKET;
				continue;
			}
			break;
		}

		freeaddrinfo(result);

		if (this->sock == INVALID_SOCKET) {
			std::cerr << "Unable to connect to server!\n";
			WSACleanup();
			return;
		}
	}
}

void NeuralClient::CloseConnection()
{
	if (this->sock != INVALID_SOCKET)
	{
		closesocket(this->sock);
		WSACleanup();
	}
}

void NeuralClient::SendData(const char * data)
{
	// Send an initial buffer
	int error = send(this->sock, data, (int)strlen(data), 0);
	if (error == SOCKET_ERROR) {
		std::cerr << "[-] NNC: Send failed with error: " << WSAGetLastError();
		closesocket(this->sock);
		WSACleanup();
		return;
	}
}

std::string NeuralClient::ReciveData()
{
	int error = 0;
	char * recvbuff = new char[1024];

	do {
		error = recv(this->sock, recvbuff, 1024, 0);

		if(error < 0)
			std::cerr << "recv failed with error: " << WSAGetLastError() << std::endl;
	} while (error > 0);

	return std::string(recvbuff);
}
