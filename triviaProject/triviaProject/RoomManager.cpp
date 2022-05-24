#include "RoomManager.h"

RoomManager::RoomManager()
{
	m_nextId = 0;
	_locker = std::unique_lock<std::mutex>(this->_roomsLock, std::defer_lock);  // sets the locker
}

void RoomManager::createRoom(LoggedUser creator, RoomData roomData)
{
	_locker.lock();
	roomData.id = m_nextId;  // updating the id to the correct one
	Room room(roomData);
	room.addUser(creator);
	m_rooms.insert(std::pair<int, Room>(roomData.id, room));

	m_nextId++;  // a room was added and so the next id will increase
	_locker.unlock();
}

void RoomManager::deleteRoom(unsigned int id)
{
	_locker.lock();
	std::map<unsigned int, Room>::iterator it = this->m_rooms.find(id);
	m_rooms.erase(it);
	_locker.unlock();
}

unsigned int RoomManager::getRoomState(unsigned int id)
{
	_locker.lock();
	unsigned int isActive = this->m_rooms[id].getRoomData().isActive;
	_locker.unlock();
	return isActive;
}

std::vector<RoomData> RoomManager::getRooms()
{
	std::vector<RoomData> roomsData;
	this->_locker.lock();
	for (const auto& room : m_rooms)  // goes through all the rooms
	{
		roomsData.push_back(room.second.getRoomData());
	}
	this->_locker.unlock();
	return roomsData;
	
}

void RoomManager::addUserToRoom(const unsigned int& id, LoggedUser newUser)
{
	this->_locker.lock();
	m_rooms[id].addUser(newUser);
	this->_locker.unlock();
}

void RoomManager::removeUserFromRoom(const unsigned int& id, LoggedUser newUser)
{
	this->_locker.lock();
	m_rooms[id].removeUser(newUser);
	this->_locker.unlock();
}

Room RoomManager::getRoom(const unsigned int& id)
{
	this->_locker.lock();
	Room roomToReturn(m_rooms[id]);
	this->_locker.unlock();
	return roomToReturn;
}

unsigned int RoomManager::getNextId()
{
	_locker.lock();
	unsigned int nextId = m_nextId;
	_locker.unlock();
	return nextId;
}

void RoomManager::setRoomState(unsigned int id, unsigned int state)
{
	this->_locker.lock();
	m_rooms[id].setIsActive(state);
	this->_locker.unlock();
}

bool RoomManager::doesRoomExist(const unsigned int& id)
{
	return m_rooms.find(id) != m_rooms.end();
}
