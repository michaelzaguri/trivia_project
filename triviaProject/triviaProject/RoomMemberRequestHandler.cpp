#include "RoomMemberRequestHandler.h"

RoomMemberRequestHandler::RoomMemberRequestHandler(LoggedUser user, RequestHandlerFactory& handleFactory, Room room)
	: m_user(user.getUsername()), m_handleFactory(handleFactory),
	m_roomManager(handleFactory.getRoomManager()), m_room(room)
{
}

bool RoomMemberRequestHandler::isRequestRelevant(RequestInfo info)
{
	return info.id == GET_ROOM_STATE_REQUEST_CODE || info.id == LEAVE_ROOM_REQUEST_CODE;
}

RequestResult RoomMemberRequestHandler::handleRequest(RequestInfo req)
{
	if (req.id == GET_ROOM_STATE_REQUEST_CODE)
	{
		return getRoomState(req);
	}
	else if (req.id == LEAVE_ROOM_REQUEST_CODE)
	{
		return leaveRoom(req);
	}
	return RequestResult();
}

RequestResult RoomMemberRequestHandler::getRoomState(RequestInfo req)
{
	RequestResult result;
	result.newHandler = this;

	try
	{
		if (m_roomManager.doesRoomExist(m_room.getRoomData().id))  // checks if the room exists
		{
			if (m_roomManager.getRoomState(m_room.getRoomData().id) == 1)  // checks if the game has started
			{
				result.newHandler = nullptr;
				result.response = JsonResponsePacketSerializer::serializeResponse(StartGameResponse{ 1 });
				// if the game has started, it returns a StartGameResponse
			}
			else
			{
				m_room.setMetadata(m_roomManager.getRoom(m_room.getRoomData().id).getRoomData());
				RoomData r_data = m_room.getRoomData();
				GetRoomStateResponse getRoomStateRes{ 1, r_data.isActive,
					m_roomManager.getRoom(m_room.getRoomData().id).getAllUsers(), r_data.numOfQuestionsInGame, r_data.timePerQuestion };
				result.response = JsonResponsePacketSerializer::serializeResponse(getRoomStateRes);
				// if the game hasent started yet, it returns a GetRoomStateResponse
			}
		}
		else
		{
			return leaveRoom(req);  // if the room doesnt exist, it returns a LeaveRoomResponse
		}
	}
	catch (...)
	{
		result.response = JsonResponsePacketSerializer::serializeResponse(ErrorResponse{ "Error while getting the room's state" });
	}
	return result;
}

RequestResult RoomMemberRequestHandler::leaveRoom(RequestInfo)
{
	RequestResult result;
	result.newHandler = m_handleFactory.createMenuRequestHandler(m_user);  // if you leave the room, you are back to the menu
	if (m_roomManager.doesRoomExist(m_room.getRoomData().id))
	{
		m_roomManager.removeUserFromRoom(m_room.getRoomData().id, m_user);
	}
	try
	{
		result.response = JsonResponsePacketSerializer::serializeResponse(LeaveRoomResponse{ 1 });
	}
	catch (...)
	{
		result.response = JsonResponsePacketSerializer::serializeResponse(ErrorResponse{ "Error while getting the room's state" });
	}
	return result;
}
