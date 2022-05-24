#pragma once
#include "IRequestHandler.h"
#include "Room.h"
#include "RoomManager.h"
#include "RequestHandlerFactory.h"

#define CLOSE_ROOM_REQUEST_CODE "10001010"
#define START_GAME_REQUEST_CODE "10001011"
#define GET_ROOM_STATE_REQUEST_CODE "10001100"

class Room;
class LoggedUser;
class RoomManager;
class RequestHandlerFactory;
class RoomAdminRequestHandler :
    public IRequestHandler
{
public:
    RoomAdminRequestHandler(LoggedUser user, RequestHandlerFactory& handleFactory, Room room);  // c'tour

    virtual bool isRequestRelevant(RequestInfo);  // checks if the request is relevant to the handler
    virtual RequestResult handleRequest(RequestInfo);  // handles the request and returns the response
    
private:
    Room m_room;  // the room the player's in
    LoggedUser m_user;  // the user
    RoomManager& m_roomManager;  // the room manager
    RequestHandlerFactory& m_handleFactory;  // the handle factory

    RequestResult closeRoom(RequestInfo);  // closes the room
    RequestResult startGame(RequestInfo);  // starts the game
    RequestResult getRoomState(RequestInfo);  // gets the room's state
};

