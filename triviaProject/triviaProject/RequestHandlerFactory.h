#pragma once
#include "LoginManager.h"
#include "LoginRequestHandler.h"
#include "MenuRequestHandler.h"
#include "RoomAdminRequestHandler.h"
#include "RoomMemberRequestHandler.h"

class LoginRequestHandler;
class LoginManager;
class StatisticsManager;
class RoomManager;
class MenuRequestHandler;
class RoomAdminRequestHandler;
class RoomMemberRequestHandler;
class RequestHandlerFactory
{
public:
	RequestHandlerFactory(IDatabase* dataBase);  // c'tour

	LoginRequestHandler* createLoginRequestHandler();  // creates a loginRequestHandler
	MenuRequestHandler* createMenuRequestHandler(LoggedUser& user);  // creates a menuRequestHandler
	LoginManager& getLoginManager();  // gets the login manager
	StatisticsManager& getStatisticsManager();  // gets the statistics manager
	RoomManager& getRoomManager();  // gets the room manager

	// v3.0.0
	RoomAdminRequestHandler* createRoomAdminRequestHandler(LoggedUser& user,Room room);  // creates a RoomAdminRequestHandler
	RoomMemberRequestHandler* createRoomMemberRequestHandler(LoggedUser& user,Room room);  // creates a RoomAdminRequestHandler

private:
	LoginManager m_loginManager;  // the login manager
	IDatabase* m_dataBase;  // the database
	RoomManager m_roomManager;  // the room manager
	StatisticsManager m_statisticsManager;  // the statistics manager
};

