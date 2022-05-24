#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include "json.hpp"
#include "Request.h"

using nlohmann::json;  // for convenience 

#define LENGTH_OF_BYTE 8
#define BINARY_NUMBER 2

class JsonRequestPacketDeserializer
{
public:
	static LoginRequest deserializeLoginRequest(std::vector<unsigned char> buffer);  // desirializes login request
	static SignupRequest deserializeSignUpRequest(std::vector<unsigned char> buffer);  // desirializes signup request
	static GetPlayersInRoomRequest deserializeGetPlayersRequest(std::vector<unsigned char> buffer);  // desirializes a get players request
	static JoinRoomRequest deserializeJoinRoomRequest(std::vector<unsigned char> buffer);  // desirializes a join room request
	static CreateRoomRequest deserializeCreateRoomRequest(std::vector<unsigned char> buffer);  // desirializes a create room request
private:
	static json getJsonString(std::vector<unsigned char> buffer);  // converts from bit array to json string
};