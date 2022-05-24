#include "RoomAdminRequestHandler.h"

RoomAdminRequestHandler::RoomAdminRequestHandler(LoggedUser user, RequestHandlerFactory& handleFactory, Room room)
	: m_user(user.getUsername()), m_handleFactory(handleFactory),
	m_roomManager(handleFactory.getRoomManager()), m_room(room)
{
}

bool RoomAdminRequestHandler::isRequestRelevant(RequestInfo info)
{
	return info.id == CLOSE_ROOM_REQUEST_CODE || info.id == START_GAME_REQUEST_CODE || info.id == GET_ROOM_STATE_REQUEST_CODE;
}

RequestResult RoomAdminRequestHandler::handleRequest(RequestInfo req)
{
	if (req.id == CLOSE_ROOM_REQUEST_CODE)
	{
		return closeRoom(req);
	}
	else if (req.id == START_GAME_REQUEST_CODE)
	{
		return startGame(req);
	}
	else if (req.id == GET_ROOM_STATE_REQUEST_CODE)
	{
		return getRoomState(req);
	}
	return RequestResult();
}

RequestResult RoomAdminRequestHandler::closeRoom(RequestInfo)
{
	RequestResult result;
	result.newHandler = m_handleFactory.createMenuRequestHandler(m_user);  // if the room is closed, youre back at the menu
	try
	{
		m_roomManager.setRoomState(m_room.getRoomData().id, 0);
		m_roomManager.deleteRoom(m_room.getRoomData().id);
		result.response = JsonResponsePacketSerializer::serializeResponse(CloseRoomResponse{1});
	}
	catch (...)
	{
		result.response = JsonResponsePacketSerializer::serializeResponse(ErrorResponse{ "Error while closing the room" });
	}
	return result;
}

RequestResult RoomAdminRequestHandler::startGame(RequestInfo)
{
	RequestResult result;
	result.newHandler = nullptr;  // the newHandler is talked about in v4

	try
	{
		m_roomManager.setRoomState(m_room.getRoomData().id, 1);
		result.response = JsonResponsePacketSerializer::serializeResponse(StartGameResponse{1});
	}
	catch (...)
	{
		result.response = JsonResponsePacketSerializer::serializeResponse(ErrorResponse{ "Error while starting the game" });
	}
	return result;
}

RequestResult RoomAdminRequestHandler::getRoomState(RequestInfo)
{
	RequestResult result;
	result.newHandler = this;
	
	try
	{
		m_room.setMetadata(m_roomManager.getRoom(m_room.getRoomData().id).getRoomData());
		RoomData r_data = m_room.getRoomData();
		GetRoomStateResponse getRoomStateRes{ 1, r_data.isActive,
			m_roomManager.getRoom(m_room.getRoomData().id).getAllUsers(), r_data.numOfQuestionsInGame, r_data.timePerQuestion};
		result.response = JsonResponsePacketSerializer::serializeResponse(getRoomStateRes);
	}
	catch (...)
	{
		result.response = JsonResponsePacketSerializer::serializeResponse(ErrorResponse{ "Error while getting the room's state" });
	}
	return result;
}
