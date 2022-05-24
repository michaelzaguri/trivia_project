#include "Server.h"

Server::Server(IDatabase* dataBase) : m_handleFactory(dataBase), m_communicator(&m_handleFactory)
{
    m_dataBase = dataBase;
}

void Server::run()
{
    std::thread t([=] {m_communicator.startHandleRequests(); });
    t.detach();
}
