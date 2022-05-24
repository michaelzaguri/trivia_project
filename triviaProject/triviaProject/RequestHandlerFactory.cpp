#include "RequestHandlerFactory.h"

RequestHandlerFactory::RequestHandlerFactory(IDatabase* dataBase) : m_loginManager(dataBase), m_statisticsManager(dataBase)
{
    m_dataBase = dataBase;
}

LoginRequestHandler* RequestHandlerFactory::createLoginRequestHandler()
{
    return new LoginRequestHandler(this);
}

MenuRequestHandler* RequestHandlerFactory::createMenuRequestHandler(LoggedUser& user)
{
    return new MenuRequestHandler(user, *(this));
}

LoginManager& RequestHandlerFactory::getLoginManager()
{
    return m_loginManager;
}

StatisticsManager& RequestHandlerFactory::getStatisticsManager()
{
    return m_statisticsManager;
}

RoomManager& RequestHandlerFactory::getRoomManager()
{
    return m_roomManager;
}

RoomAdminRequestHandler* RequestHandlerFactory::createRoomAdminRequestHandler(LoggedUser& user, Room room)
{
    return new RoomAdminRequestHandler(user, *(this), room);
}

RoomMemberRequestHandler* RequestHandlerFactory::createRoomMemberRequestHandler(LoggedUser& user, Room room)
{
    return new RoomMemberRequestHandler(user, *(this), room);
}
