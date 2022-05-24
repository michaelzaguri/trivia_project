#include "JsonPacketSerializer.h"

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(ErrorResponse error)
{
	std::vector<unsigned char> buffer;
	json response;
	response["message"] = error.message;

	std::string resStr = response.dump();  // getting the json string to serialize it

	serializeCode(ERROR_CODE, buffer);
	serializeLenght(resStr.length(), buffer);
	serializeContent(resStr, buffer);

	return buffer;
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(LoginResponse login)
{
	std::vector<unsigned char> buffer;
	json response;
	response["status"] = login.status;

	std::string resStr = response.dump();  // getting the json string to serialize it

	serializeCode(LOGIN_CODE, buffer);
	serializeLenght(resStr.length(), buffer);
	serializeContent(resStr, buffer);

	return buffer;
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(SignupResponse signup)
{
	std::vector<unsigned char> buffer;
	json response;
	response["status"] = signup.status;

	std::string resStr = response.dump();  // getting the json string to serialize it

	serializeCode(SIGNUP_CODE, buffer);
	serializeLenght(resStr.length(), buffer);
	serializeContent(resStr, buffer);

	return buffer;
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(LogoutResponse logout)
{
	std::vector<unsigned char> buffer;
	json response;
	response["status"] = logout.status;

	std::string resStr = response.dump();  // getting the json string to serialize it

	serializeCode(LOGOUT_CODE, buffer);
	serializeLenght(resStr.length(), buffer);
	serializeContent(resStr, buffer);

	return buffer;
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(GetRoomsResponse getRoom)
{
	std::vector<unsigned char> buffer;
	json response;
	response["status"] = getRoom.status;

	json roomsJson = getRoom.rooms;
	response["rooms"] = roomsJson;

	std::string resStr = response.dump();  // getting the json string to serialize it

	serializeCode(GET_ROOMS_CODE, buffer);
	serializeLenght(resStr.length(), buffer);
	serializeContent(resStr, buffer);

	return buffer;
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(GetPlayersInRoomResponse playersInRoom)
{
	std::vector<unsigned char> buffer;
	json response;

	json playersJson = playersInRoom.players;
	response["players"] = playersJson;

	std::string resStr = response.dump();  // getting the json string to serialize it

	serializeCode(GET_PLAYERS_CODE, buffer);
	serializeLenght(resStr.length(), buffer);
	serializeContent(resStr, buffer);

	return buffer;
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(JoinRoomResponse joinRoom)
{
	std::vector<unsigned char> buffer;
	json response;
	response["status"] = joinRoom.status;

	std::string resStr = response.dump();  // getting the json string to serialize it

	serializeCode(JOIN_ROOM_CODE, buffer);
	serializeLenght(resStr.length(), buffer);
	serializeContent(resStr, buffer);

	return buffer;
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(CreateRoomResponse createRoom)
{
	std::vector<unsigned char> buffer;
	json response;
	response["status"] = createRoom.status;

	std::string resStr = response.dump();  // getting the json string to serialize it

	serializeCode(CREATE_ROOM_CODE, buffer);
	serializeLenght(resStr.length(), buffer);
	serializeContent(resStr, buffer);

	return buffer;
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(GetHighScoreResponse getHighScore)
{
	std::vector<unsigned char> buffer;
	json response;
	response["status"] = getHighScore.status;

	json statsJson = getHighScore.statistics;
	response["statistics"] = statsJson;

	std::string resStr = response.dump();  // getting the json string to serialize it

	serializeCode(GET_HIGH_SCORE_CODE, buffer);
	serializeLenght(resStr.length(), buffer);
	serializeContent(resStr, buffer);

	return buffer;
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(GetPersonalStatsResponse getPersonalStats)
{
	std::vector<unsigned char> buffer;
	json response;
	response["status"] = getPersonalStats.status;

	json statsJson = getPersonalStats.statistics;
	response["statistics"] = statsJson;

	std::string resStr = response.dump();  // getting the json string to serialize it

	serializeCode(GET_PERSONAL_STATS_CODE, buffer);
	serializeLenght(resStr.length(), buffer);
	serializeContent(resStr, buffer);

	return buffer;
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(CloseRoomResponse closeRoom)
{
	std::vector<unsigned char> buffer;
	json response;
	response["status"] = closeRoom.status;

	std::string resStr = response.dump();  // getting the json string to serialize it

	serializeCode(CLOSE_ROOM_CODE, buffer);
	serializeLenght(resStr.length(), buffer);
	serializeContent(resStr, buffer);

	return buffer;
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(StartGameResponse startGame)
{
	std::vector<unsigned char> buffer;
	json response;
	response["status"] = startGame.status;

	std::string resStr = response.dump();  // getting the json string to serialize it

	serializeCode(START_GAME_CODE, buffer);
	serializeLenght(resStr.length(), buffer);
	serializeContent(resStr, buffer);

	return buffer;
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(GetRoomStateResponse getRoomState)
{
	std::vector<unsigned char> buffer;
	json response;
	response["status"] = getRoomState.status;
	response["hasGameBegone"] = getRoomState.hasGameBegun;

	json playersJson = getRoomState.players;
	response["players"] = playersJson;

	response["questionCount"] = getRoomState.questionCount;
	response["answerTimeout"] = getRoomState.answerTimeout;

	std::string resStr = response.dump();  // getting the json string to serialize it

	serializeCode(GET_ROOM_STATE_CODE, buffer);
	serializeLenght(resStr.length(), buffer);
	serializeContent(resStr, buffer);

	return buffer;
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(LeaveRoomResponse leaveRoom)
{
	std::vector<unsigned char> buffer;
	json response;
	response["status"] = leaveRoom.status;

	std::string resStr = response.dump();  // getting the json string to serialize it

	serializeCode(LEAVE_ROOM_CODE, buffer);
	serializeLenght(resStr.length(), buffer);
	serializeContent(resStr, buffer);

	return buffer;
}

void JsonResponsePacketSerializer::serializeCode(const int& code, std::vector<unsigned char>& buffer)
{
	std::bitset<BYTE> bitCode(code);  // turning the code into a bitset
	std::string bitStr = bitCode.to_string();  // saving the bitset as a str
	int i = 0;

	for (int i = 0; i < BYTE; i++)
	{
		buffer.push_back(bitStr[i]);  // adds every bit to the buffer
	}
}

void JsonResponsePacketSerializer::serializeLenght(const int& length, std::vector<unsigned char>& buffer)
{
	std::bitset<LENGTH> bitLength(length);  // turning the length into a bitset
	std::string bitStr = bitLength.to_string();  // saving the bitset as a str
	int i = 0;

	for (int i = 0; i < LENGTH; i++)
	{
		buffer.push_back(bitStr[i]);  // adds every bit to the buffer
	}
}

void JsonResponsePacketSerializer::serializeContent(const std::string content, std::vector<unsigned char>& buffer)
{
	int i = 0, j = 0;

	for (i = 0; i < content.length(); i++)  // going through all the chars in the content
	{
		std::bitset<BYTE> charBits((int)content[i]);  // turning the char into a bitset
		std::string bitsStr = charBits.to_string();  // saving the bitset as a str

		for (j = 0; j < BYTE; j++)
		{
			buffer.push_back(bitsStr[j]);  // adds every bit to the buffer
		}
	}
}
