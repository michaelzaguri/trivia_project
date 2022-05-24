#include "MenuRequestHandler.h"

MenuRequestHandler::MenuRequestHandler(LoggedUser user, RequestHandlerFactory& handleFactory)
	: m_user(user.getUsername()), m_handleFactory(handleFactory), 
	m_roomManager(handleFactory.getRoomManager()), m_statisticsManager(handleFactory.getStatisticsManager())
{
}

bool MenuRequestHandler::isRequestRelevant(RequestInfo info)
{
	return info.id == CREATE_ROOM_REQUEST || info.id == GET_ROOMS_REQUEST ||
		info.id == GET_PLAYERS_IN_ROOM_REQUEST || info.id == JOIN_ROOM_REQUEST ||
		info.id == GET_HIGH_SCORE_REQUEST || info.id == GET_PERSONAL_STATS_REQUEST ||
		info.id == LOGOUT_REQUEST;
}

RequestResult MenuRequestHandler::handleRequest(RequestInfo req)
{
	if (req.id == CREATE_ROOM_REQUEST)
	{
		return createRoom(req);
	}
	else if (req.id == GET_ROOMS_REQUEST)
	{
		return getRooms(req);
	}
	else if (req.id == GET_PLAYERS_IN_ROOM_REQUEST)
	{
		return getPlayersInRoom(req);
	}
	else if (req.id == JOIN_ROOM_REQUEST)
	{
		return joinRoom(req);
	}
	else if (req.id == GET_HIGH_SCORE_REQUEST)
	{
		return getHighScore(req);
	}
	else if (req.id == GET_PERSONAL_STATS_REQUEST)
	{
		return getPersonalStats(req);
	}
	else if (req.id == LOGOUT_REQUEST)
	{
		return signout(req);
	}
	return RequestResult();
}

RequestResult MenuRequestHandler::signout(const RequestInfo& info)
{
	RequestResult result;
	result.newHandler = nullptr;

	bool success = m_handleFactory.getLoginManager().logout(m_user.getUsername());

	if (success)
	{
		result.response = JsonResponsePacketSerializer::serializeResponse(LogoutResponse{ 1 });
		return result;
	}

	result.response = JsonResponsePacketSerializer::serializeResponse(ErrorResponse{ "Error while logging out" });
	result.newHandler = this;
	return result;
}

RequestResult MenuRequestHandler::getRooms(const RequestInfo& info)
{
	RequestResult result;
	result.newHandler = this;

	try
	{
		result.response = JsonResponsePacketSerializer::serializeResponse(GetRoomsResponse{ 1, m_roomManager.getRooms() });
	}
	catch (...)
	{
		result.response = JsonResponsePacketSerializer::serializeResponse(ErrorResponse{ "Error while getting the rooms" });
	}
	return result;
}

RequestResult MenuRequestHandler::getPlayersInRoom(const RequestInfo& info)
{
	RequestResult result;
	result.newHandler = this;

	try
	{
		GetPlayersInRoomRequest playersReq = JsonRequestPacketDeserializer::deserializeGetPlayersRequest(info.buffer);

		std::vector<std::string> players = m_roomManager.getRoom(playersReq.roomId).getAllUsers();

		result.response = JsonResponsePacketSerializer::serializeResponse(GetPlayersInRoomResponse{ players });
	}
	catch (...)
	{
		result.response = JsonResponsePacketSerializer::serializeResponse(
			ErrorResponse{ "Error while getting the players in the room" });
	}

	return result;
}

RequestResult MenuRequestHandler::getPersonalStats(const RequestInfo& info)
{
	RequestResult result;
	result.newHandler = this;

	try
	{
		result.response = JsonResponsePacketSerializer::serializeResponse(GetPersonalStatsResponse{ 1,
		m_statisticsManager.getUserStatistics(m_user.getUsername()) });
	}
	catch (...)
	{
		result.response = JsonResponsePacketSerializer::serializeResponse(ErrorResponse{ "Error while getting the personal stats" });
	}

	return result;
}

RequestResult MenuRequestHandler::getHighScore(const RequestInfo& info)
{
	RequestResult result;
	result.newHandler = this;

	try
	{
		result.response = JsonResponsePacketSerializer::serializeResponse(GetHighScoreResponse{ 1,
		m_statisticsManager.getHighScore() });
	}
	catch (...)
	{
		result.response = JsonResponsePacketSerializer::serializeResponse(ErrorResponse{ "Error while getting the high score" });
	}

	return result;
}

RequestResult MenuRequestHandler::joinRoom(const RequestInfo& info)
{
	RequestResult result;
	result.newHandler = this;
	
	try
	{
		JoinRoomRequest joinRoomReq = JsonRequestPacketDeserializer::deserializeJoinRoomRequest(info.buffer);
		m_roomManager.addUserToRoom(joinRoomReq.roomId, m_user);
		result.newHandler = m_handleFactory.createRoomMemberRequestHandler(m_user, m_roomManager.getRoom(joinRoomReq.roomId));
		result.response = JsonResponsePacketSerializer::serializeResponse(JoinRoomResponse{ 1 });
	}
	catch (...)
	{
		result.response = JsonResponsePacketSerializer::serializeResponse(ErrorResponse{ "Error while joining the room" });
	}

	return result;
}

RequestResult MenuRequestHandler::createRoom(const RequestInfo& info)
{
	RequestResult result;
	result.newHandler = this;

	try
	{
		int roomId = m_roomManager.getNextId();
		CreateRoomRequest createRoomReq = JsonRequestPacketDeserializer::deserializeCreateRoomRequest(info.buffer);
		RoomData rData{ 0, createRoomReq.roomName, createRoomReq.maxUsers, createRoomReq.questionCount, createRoomReq.answerTimeout, 0 };
		m_roomManager.createRoom(m_user, rData);

		result.newHandler = m_handleFactory.createRoomAdminRequestHandler(m_user, m_roomManager.getRoom(roomId));

		result.response = JsonResponsePacketSerializer::serializeResponse(CreateRoomResponse{ 1 });
	}
	catch (...)
	{
		result.response = JsonResponsePacketSerializer::serializeResponse(ErrorResponse{ "Error while creating a room" });
	}

	return result;
}
