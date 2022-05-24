#include "Communicator.h"


Communicator::Communicator(RequestHandlerFactory* factory)
{
	// this server use TCP. that why SOCK_STREAM & IPPROTO_TCP
	// if the server use UDP we will use: SOCK_DGRAM & IPPROTO_UDP
	m_serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (m_serverSocket == INVALID_SOCKET)
		throw std::exception(__FUNCTION__ " - socket");

	m_handleFactory = factory;
}

Communicator::~Communicator()
{
	try
	{
		// the only use of the destructor should be for freeing 
		// resources that was allocated in the constructor
		closesocket(m_serverSocket);
	}
	catch (...) {}
}

void Communicator::startHandleRequests()
{
	bindAndListen();

	while (true)
	{
		// the main thread is only accepting clients 
		// and add then to the list of handlers
		std::cout << "Waiting for client connection request" << std::endl;

		// this accepts the client and create a specific socket from server to this client
		SOCKET client_socket = ::accept(m_serverSocket, NULL, NULL);

		if (client_socket == INVALID_SOCKET)
			throw std::exception(__FUNCTION__);

		std::cout << "Client accepted. Server and client can speak" << std::endl;

		LoginRequestHandler* loginHandler = new LoginRequestHandler(this->m_handleFactory);

		m_clients.insert(std::pair<SOCKET, IRequestHandler*>(client_socket, loginHandler));

		// the function that handle the conversation with the client
		std::thread t([=] {handleNewClient(client_socket); });
		t.detach();
	}
}

void Communicator::bindAndListen()
{
	struct sockaddr_in sa = { 0 };

	sa.sin_port = htons(SERVER_PORT); // port that server will listen for
	sa.sin_family = AF_INET;   // must be AF_INET
	sa.sin_addr.s_addr = INADDR_ANY;    // when there are few ip's for the machine. We will use always "INADDR_ANY"

	// again stepping out to the global namespace
	// Connects between the socket and the configuration (port and etc..)
	if (bind(m_serverSocket, (struct sockaddr*)&sa, sizeof(sa)) == SOCKET_ERROR)
		throw std::exception(__FUNCTION__ " - bind");
	std::cout << "server binded" << std::endl;

	// Start listening for incoming requests of clients
	if (listen(m_serverSocket, SOMAXCONN) == SOCKET_ERROR)
		throw std::exception(__FUNCTION__ " - listen");
	std::cout << "Listening on port " << SERVER_PORT << std::endl;
}

void Communicator::handleNewClient(SOCKET clientSoc)
{
	try
	{
		while (true)  // we might need to add another conditional to ensure the client sock is closed after the user logs out
		{
			std::string code = gettingStringValue(BYTE_LEN, clientSoc);  // getting the code of the message

			int length = gettingIntValue(SIZE_LEN, clientSoc) * BYTE_LEN;  // getting the length of the message

			std::string messageStr;
			if (length)
			{
				messageStr = gettingStringValue(length, clientSoc);  // if the length is 0 then there is no message to recieve
			}

			RequestInfo info;
			info.id = code.substr(0, BYTE_LEN);
			info.receivalTime = time(NULL);
			info.buffer = std::vector<unsigned char>(messageStr.begin(), messageStr.end());
			if (m_clients[clientSoc]->isRequestRelevant(info))
			{
				RequestResult result = m_clients[clientSoc]->handleRequest(info);  // getting the result

				if (send(clientSoc, std::string(result.response.begin(),
					result.response.end()).c_str(), result.response.size(), 0) == INVALID_SOCKET)
				{
					throw std::exception("Error while sending response to client");
				}
				m_clients[clientSoc] = result.newHandler;  // updating the handler
			}

		}
	}
	catch (...)
	{
		closesocket(clientSoc);
	}
}

std::string Communicator::gettingStringValue(unsigned int size, SOCKET soc)
{
	char* data = new char[size + 1];
	int res = recv(soc, data, size, 0);
	data[size] = 0;
	if (res == INVALID_SOCKET)
	{
		std::string s = "Error while recieving from socket: ";
		s += std::to_string(soc);
		throw std::exception(s.c_str());
	}
	return std::string(data);
}

int Communicator::gettingIntValue(unsigned int size, SOCKET soc)
{
	return std::stoi(gettingStringValue(size, soc), nullptr, BINARY_NUMBER);  // converting the string into a number
}
