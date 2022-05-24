#include "JsonRequestPacketDeserializer.h"

LoginRequest JsonRequestPacketDeserializer::deserializeLoginRequest(std::vector<unsigned char> buffer)
{
    json jsStr = getJsonString(buffer);
    return LoginRequest{ jsStr["Username"].get<std::string>(), jsStr["Password"].get<std::string>() };
}

SignupRequest JsonRequestPacketDeserializer::deserializeSignUpRequest(std::vector<unsigned char> buffer)
{
    json jsStr = getJsonString(buffer);
    return SignupRequest{ jsStr["Username"].get<std::string>(), jsStr["Password"].get<std::string>(), jsStr["Email"].get<std::string>() };
}

GetPlayersInRoomRequest JsonRequestPacketDeserializer::deserializeGetPlayersRequest(std::vector<unsigned char> buffer)
{
    json jsStr = getJsonString(buffer);
    return GetPlayersInRoomRequest{ jsStr["roomId"].get<unsigned int>() };
}

JoinRoomRequest JsonRequestPacketDeserializer::deserializeJoinRoomRequest(std::vector<unsigned char> buffer)
{
    json jsStr = getJsonString(buffer);
    return JoinRoomRequest{ jsStr["roomId"].get<unsigned int>() };
}

CreateRoomRequest JsonRequestPacketDeserializer::deserializeCreateRoomRequest(std::vector<unsigned char> buffer)
{
    json jsStr = getJsonString(buffer);
    return CreateRoomRequest{ jsStr["roomName"].get<std::string>(), jsStr["maxUsers"].get<unsigned int>(), jsStr["questionCount"].get<unsigned int>(), jsStr["answerTimeout"].get<unsigned int>() };
}

json JsonRequestPacketDeserializer::getJsonString(std::vector<unsigned char> buffer)
{
    int i = 0;
    std::string realValues = "";
    std::string binaryValues(buffer.begin(), buffer.end());//turning the vector into a string so it will be easier to play with
    for (i = 0; i < binaryValues.length(); i += LENGTH_OF_BYTE)
    {
        std::string curr(binaryValues.begin() + i, binaryValues.begin() + i + LENGTH_OF_BYTE);// the size of a 1 char is a byte so we take a byte from the binarystring and save it on a string
        realValues += std::stoi(curr, nullptr, BINARY_NUMBER);// turning the binary value of a char into an number which will later be converted into a char by adding a number to a string
    }
    return json::parse(realValues);// parse string value into a json string value
}
