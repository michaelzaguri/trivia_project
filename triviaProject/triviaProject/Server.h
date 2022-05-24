#pragma once
#include "Communicator.h"
#include "RequestHandlerFactory.h"
#include "IDatabase.h"

class Server
{
public:
	Server(IDatabase* dataBase);  // c'tour
	// add a d'tour later

	void run();  // runs the server

private:
	Communicator m_communicator;  // the communicator
	IDatabase* m_dataBase;  // the database
	RequestHandlerFactory m_handleFactory;  // the handlerFactory
};

