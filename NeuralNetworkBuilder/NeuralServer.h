/****************************************************************************************************************/
/*                                                                                                              */
/*   NNE - Neural Networks Engine                                                                               */
/*   www.sapirshemer.com                                                                                        */
/*                                                                                                              */
/*   Neural Server HEADER                                                                                       */
/*                                                                                                              */
/*   Sapir Shemer                                                                                               */
/*   DevShift - Shifting development forward                                                                    */
/*   me@sapirshemer.com                                                                                         */
/*                                                                                                              */
/****************************************************************************************************************/

#pragma once
#include "definitions.h"
#include "NeuralClient.h"

class NeuralServer
{
public:
	NeuralServer(const char * port);
	~NeuralServer();

	// Start server
	void RunServer();
	// Send data to server
	void SendData(const char * data);
	// Manage a machine
	static void ManageMachine(SOCKET socket);
	// Mange client
	static void ManageClient(SOCKET socket);
private:
	const char * port;
	SOCKET ListenSocket = INVALID_SOCKET;
	SOCKET ClientSocket = INVALID_SOCKET;
	WSADATA wsaData;
	addrinfo hints;
	char * recv_data = new char[1024];
	
	// Managed connections as threads
	std::vector<std::thread> managed_connections;
	std::vector<std::string> devices_ids;
};

