#pragma once
#include <WinSock2.h>
//#include <map>
#include <iostream>
#include <exception>
#include <thread>
#include <string>
#include "WSAInitializer.h"
#include "IRequestHandler.h"
#include "LoginRequestHandler.h"
#include "JsonRequestPacketDeserializer.h"
#include "JsonPacketSerializer.h"
#include "RequestHandlerFactory.h"

#define SERVER_PORT 2811
#define SIZE_LEN 32
#define BYTE_LEN 8

class Communicator
{
public:
	Communicator(RequestHandlerFactory* factory);  // c'tour
	~Communicator();  // d'tour

	void startHandleRequests();  // starts the communication with all the users (including the listening)

private:
	SOCKET m_serverSocket;  // the socket of the server
	std::map<SOCKET, IRequestHandler*> m_clients;  // the map of all the clients
	RequestHandlerFactory* m_handleFactory;  // the handle factory

	void bindAndListen();  // binds the communication and listens
	void handleNewClient(SOCKET clientSoc);  // the function for the communication itself with the client once they are accepted

	std::string gettingStringValue(unsigned int size, SOCKET soc);  // getting a string from the socket
	int gettingIntValue(unsigned int size, SOCKET soc);  // getting an int from the socket
};

