#pragma once
#include "IRequestHandler.h"
#include "RoomManager.h"
#include "StatisticsManager.h"
#include "RequestHandlerFactory.h"
#include "JsonPacketSerializer.h"
#include "JsonRequestPacketDeserializer.h"

#define CREATE_ROOM_REQUEST "10000111"
#define GET_ROOMS_REQUEST "10000100"
#define GET_PLAYERS_IN_ROOM_REQUEST "10000101"
#define JOIN_ROOM_REQUEST "10000110"
#define GET_HIGH_SCORE_REQUEST "10001000"
#define GET_PERSONAL_STATS_REQUEST "10001001"
#define LOGOUT_REQUEST "10000011"

class RoomManager;
class StatisticsManager;
class RequestHandlerFactory;
class MenuRequestHandler :
    public IRequestHandler
{
public:
    MenuRequestHandler(LoggedUser user, RequestHandlerFactory& handleFactory);  // c'tour

    virtual bool isRequestRelevant(RequestInfo);  // checks if the request is relevant to the handler
    virtual RequestResult handleRequest(RequestInfo);  // handles the request and returns the response

private:
    LoggedUser m_user;  // the user itself
    RoomManager& m_roomManager;  // the room manager
    StatisticsManager& m_statisticsManager;  // the statistics manager
    RequestHandlerFactory& m_handleFactory;  // the handle factory

    RequestResult signout(const RequestInfo& info);  // signs out the user
    RequestResult getRooms(const RequestInfo& info);  // gets all the rooms
    RequestResult getPlayersInRoom(const RequestInfo& info);  // gets all the players in a room
    RequestResult getPersonalStats(const RequestInfo& info);  // gets the personal stats of a user
    RequestResult getHighScore(const RequestInfo& info);  // gets the highest scores in the game
    RequestResult joinRoom(const RequestInfo& info);  // joins the user into a room
    RequestResult createRoom(const RequestInfo& info);  // creates a room
};

