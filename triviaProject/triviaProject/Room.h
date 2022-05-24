#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <mutex>
#include "LoggedUser.h"
#include "json.hpp"

namespace nh = nlohmann;

struct RoomData
{
	unsigned int id;
	std::string name;
	unsigned int maxPlayers;
	unsigned int numOfQuestionsInGame;
	unsigned int timePerQuestion;
	unsigned int isActive;
};

class Room
{
public:
	Room();  // c'tour
	Room(const RoomData& data);  // c'tour
	Room(const Room& data);  // copy c'tour
	void addUser(LoggedUser);  // adds a user to the users vector
	void removeUser(LoggedUser);  // removes a user from the users vector
	std::vector<std::string> getAllUsers();  // return a vector of all the users

	// getters and setters
	RoomData getRoomData() const;
	void setIsActive(const unsigned int& isActive);
	void setMetadata(const RoomData&);

private:
	RoomData m_metadata;  // the data of the room
	std::vector<LoggedUser> m_users;  // the vector of the users in the room

	std::mutex _usersLock;  // a mutex for the users vector
	std::unique_lock<std::mutex> _locker;  // a locker for the mutex
};

