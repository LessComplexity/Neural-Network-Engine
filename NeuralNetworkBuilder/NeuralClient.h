/****************************************************************************************************************/
/*                                                                                                              */
/*   NNE - Neural Networks Engine                                                                               */
/*   www.sapirshemer.com                                                                                        */
/*                                                                                                              */
/*   Client HEADER                                                                                              */
/*                                                                                                              */
/*   Sapir Shemer                                                                                               */
/*   DevShift - Shifting development forward                                                                    */
/*   me@sapirshemer.com                                                                                         */
/*                                                                                                              */
/****************************************************************************************************************/

#pragma once
#include "definitions.h"

class NeuralClient
{
public:
	NeuralClient();
	NeuralClient(const char * server_ip, const char * port);
	~NeuralClient();

	// Connect to server
	void Connect(const char* server_ip, const char * server_port);
	void Connect();
	// Close the connection
	void CloseConnection();
	// Send data to server
	void SendData(const char * data);
	std::string ReciveData();
private:
	const char* server_ip;
	const char* server_port;
	// Socket holder
	SOCKET sock = INVALID_SOCKET;
	WSADATA wsadata;
	SOCKADDR_IN target; //Socket address information

	addrinfo hints;
};

