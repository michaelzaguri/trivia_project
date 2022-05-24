#include <iostream>
#include <string>

struct LoginRequest
{
	std::string username;  // the username
	std::string password;  // the password
};

struct SignupRequest
{
	std::string username;  // the username
	std::string password;  // the password
	std::string email;  // the email
};

struct GetPlayersInRoomRequest
{
	unsigned int roomId;  // the id of the room
};

struct JoinRoomRequest
{
	unsigned int roomId;  // the id of the room
};

struct CreateRoomRequest
{
	std::string roomName;  // the name of the roomm
	unsigned int maxUsers;  // the number of users that can be in the room
	unsigned int questionCount;  // the number of questions
	unsigned int answerTimeout;  // the timeout for each question
};