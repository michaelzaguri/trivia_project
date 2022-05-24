#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "json.hpp"
#include "Room.h"


#define SIGNUP_CODE 0
#define LOGIN_CODE 1
#define ERROR_CODE 2
#define LOGOUT_CODE 3
#define GET_ROOMS_CODE 4
#define GET_PLAYERS_CODE 5
#define JOIN_ROOM_CODE 6
#define CREATE_ROOM_CODE 7
#define GET_HIGH_SCORE_CODE 8
#define GET_PERSONAL_STATS_CODE 9
#define CLOSE_ROOM_CODE 10
#define START_GAME_CODE 11
#define GET_ROOM_STATE_CODE 12
#define LEAVE_ROOM_CODE 13

using nlohmann::json;

//User related responses

struct LoginResponse
{
	unsigned int status;  // the status of the response
};

struct SignupResponse
{
	unsigned int status;  // the status of the response
};

struct LogoutResponse
{
	unsigned int status;  // the status of the response
};

//Stats related responses

struct GetHighScoreResponse
{
	unsigned int status;  // the status of the response
	std::vector<std::string> statistics;
};

struct GetPersonalStatsResponse
{
	unsigned int status;  // the status of the response
	std::vector<std::string> statistics;  // the statistics of the user
};

//Room related responses

//version 3.0.0
struct GetRoomsResponse
{
	unsigned int status;  // the status of the response
	std::vector<RoomData> rooms;  // all of the rooms
};

struct GetPlayersInRoomResponse
{
	std::vector<std::string> players;  // all of the players in a room
};

struct JoinRoomResponse
{
	unsigned int status;  // the status of the response
};

struct CreateRoomResponse
{
	unsigned int status;  // the status of the response
};

//version 3.0.0
struct CloseRoomResponse
{
	unsigned int status;  // the status of the response
};

struct StartGameResponse
{
	unsigned int status;  // the status of the response
};

struct GetRoomStateResponse
{
	unsigned int status;  // the status of the response
	bool hasGameBegun;  // has the game begun
	std::vector<std::string> players;  // all of the players in the room
	unsigned int questionCount;  // the number of questions in the game
	unsigned int answerTimeout;  // the timeout for each question
};

struct LeaveRoomResponse
{
	unsigned int status;  // the status of the response
};

//Error related respnses

struct ErrorResponse
{
	std::string message;  // the error message
};