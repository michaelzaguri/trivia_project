#pragma once
#include <iostream>
#include <vector>
#include <bitset>
#include "Response.h"

#define BYTE 8
#define LENGTH 32

using json = nlohmann::json;  // for convenience

inline void to_json(nh::json& j, const RoomData& val)
{
	j["id"] = val.id;
	j["name"] = val.name;
	j["maxPlayers"] = val.maxPlayers;
	j["numOfQuestionsInGame"] = val.numOfQuestionsInGame;
	j["timePerQuestion"] = val.timePerQuestion;
	j["isActive"] = val.isActive;
}

inline void from_json(const nh::json& j, RoomData& val)
{
	j.at("id").get_to(val.id);
	j.at("name").get_to(val.name);
	j.at("maxPlayers").get_to(val.maxPlayers);
	j.at("numOfQuestionsInGame").get_to(val.numOfQuestionsInGame);
	j.at("timePerQuestion").get_to(val.timePerQuestion);
	j.at("isActive").get_to(val.isActive);
}

class JsonResponsePacketSerializer
{
public:
	// 1.0.0
	static std::vector<unsigned char> serializeResponse(ErrorResponse);  // serialize error response
	static std::vector<unsigned char> serializeResponse(LoginResponse);  // serialize login response
	static std::vector<unsigned char> serializeResponse(SignupResponse);  // serialize signup response

	// 2.0.0
	static std::vector<unsigned char> serializeResponse(LogoutResponse);  // serialize logout response
	static std::vector<unsigned char> serializeResponse(GetRoomsResponse);  // serialize get rooms response
	static std::vector<unsigned char> serializeResponse(GetPlayersInRoomResponse);  // serialize get players in room response
	static std::vector<unsigned char> serializeResponse(JoinRoomResponse);  // serialize join room response
	static std::vector<unsigned char> serializeResponse(CreateRoomResponse);  // serialize create room response
	static std::vector<unsigned char> serializeResponse(GetHighScoreResponse);  // serialize get high score response
	static std::vector<unsigned char> serializeResponse(GetPersonalStatsResponse);  // serialize get personal stats response

	// 3.0.0
	static std::vector<unsigned char> serializeResponse(CloseRoomResponse);  // serialize close room response
	static std::vector<unsigned char> serializeResponse(StartGameResponse);  // serialize start game response
	static std::vector<unsigned char> serializeResponse(GetRoomStateResponse);  // serialize get room state response
	static std::vector<unsigned char> serializeResponse(LeaveRoomResponse);  // serialize leave room response

private:
	static void serializeCode(const int& code, std::vector<unsigned char>& buffer);  // serializes the code of the response
	static void serializeLenght(const int& length, std::vector<unsigned char>& buffer);  
	// serializes the length of the content of the response
	static void serializeContent(const std::string content, std::vector<unsigned char>& buffer);  
	// serializes the content itself of the json
};

