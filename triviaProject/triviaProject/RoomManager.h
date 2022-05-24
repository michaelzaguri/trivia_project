#pragma once
#include <map>
#include <vector>
#include <mutex>
#include "Room.h"

class RoomManager
{
public:
	RoomManager();  // c'tour

	void createRoom(LoggedUser creator, RoomData roomData);  // creates a room
	void deleteRoom(unsigned int id);  // deletes a room
	unsigned int getRoomState(unsigned int id);  // gets if a room is active
	std::vector<RoomData> getRooms();  // gets all of the rooms
	Room getRoom(const unsigned int& id);  // gets a room
	unsigned int getNextId();  // gets m_nextId

	void addUserToRoom(const unsigned int& id, LoggedUser newUser);  // adds a user to the room
	void removeUserFromRoom(const unsigned int& id, LoggedUser newUser);  // removes a user from a room
	
	void setRoomState(unsigned int, unsigned int);  // sets for a room if it's active
	bool doesRoomExist(const unsigned int& id);  // checks if a room exists in m_rooms

private:
	std::map<unsigned int, Room> m_rooms;  // all of the rooms in the database

	unsigned int m_nextId;  // the next id for a new room
	std::mutex _roomsLock;  // the locker of the class
	std::unique_lock<std::mutex> _locker;  // a locker for the mutex
};

