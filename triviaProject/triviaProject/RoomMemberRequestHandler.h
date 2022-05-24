#pragma once
#include "IRequestHandler.h"
#include "Room.h"
#include "RoomManager.h"
#include "RequestHandlerFactory.h"

//#define GET_ROOM_STATE_REQUEST_CODE "10001100"
#define LEAVE_ROOM_REQUEST_CODE "10001101"

class Room;
class LoggedUser;
class RoomManager;
class RequestHandlerFactory;
class RoomMemberRequestHandler :
    public IRequestHandler
{
public:
    RoomMemberRequestHandler(LoggedUser user, RequestHandlerFactory& handleFactory, Room room);  // c'tour

    virtual bool isRequestRelevant(RequestInfo);  // checks if the request is relevant to the handler
    virtual RequestResult handleRequest(RequestInfo);  // handles the request and returns the response

private:
    Room m_room;
    LoggedUser m_user;
    RoomManager& m_roomManager;
    RequestHandlerFactory& m_handleFactory;

    RequestResult getRoomState(RequestInfo);  // gets the room's state
    RequestResult leaveRoom(RequestInfo);  // leaves the room
};

